/*
	ps2.c
	
	Handles the low-level parts of the PS/2 protocol
	I.e. buffers and bit-bang state machine
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "settings.h"
#include "system.h"
#include "scancode.h"

__xdata ps2port ports[2];

void InitPS2Ports(void)
{
	memset(ports, 0x00, sizeof(ports));
	ports[0].data = 0xFF;
	ports[0].parity = 1;

	ports[1].data = 0xFF;
	ports[1].parity = 1;
}

bool ReadPS2Clock(uint8_t port)
{
	if (port == PORT_KEY)
		return KEY_CLOCK & 
		(FlashSettings->EnableAUXPS2 ? KEYAUX_CLOCK : 1); // If AUX port enabled, consider that too
	else {/*if (port == PORT_MOUSE)*/
		return MOUSE_CLOCK &
		(FlashSettings->EnableAUXPS2 ? MOUSEAUX_CLOCK : 1); // If AUX port enabled, consider that too
	}
}

bool ReadPS2Data(uint8_t port)
{
	if (port == PORT_KEY)
		return KEY_DATA & 
		(FlashSettings->EnableAUXPS2 ? KEYAUX_DATA : 1); // If AUX port enabled, consider that too
	else { /*if (port == PORT_MOUSE)*/
#if defined(BOARD_AXP) || defined(BOARD_MINI)
		if (
			(P4_IN & 0b00001000) && 
			!(FlashSettings->EnableAUXPS2 && !MOUSEAUX_DATA) // If AUX port enabled, consider that too
			)
			return 1;
		else 
			return 0;

#else
		return MOUSE_DATA & 
		(FlashSettings->EnableAUXPS2 ? MOUSEAUX_DATA : 1);
#endif
	}

}

void SimonSaysSendKeyboard(__code uint8_t *chunk)
{
	if (chunk != NULL &&														 // chunk is valid
		((ports[PORT_KEY].sendBuffEnd + 1)  & 0x3F) != ports[PORT_KEY].sendBuffStart) // not full
	{
		ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffEnd] = chunk;
		ports[PORT_KEY].sendBuffEnd = (ports[PORT_KEY].sendBuffEnd + 1) & 0x3F;
	}
}

__xdata char buf[100];

bool SendKeyboard(__code uint8_t *chunk)
{
	// reset watchdog timer, most calls of this routine block. They shouldn't really
	SoftWatchdog = 0;
	// chunk is null, pretend we sent it
	if (chunk == NULL)
		return 1;

	// If we're emulating an 83-key keyboard, don't send keycodes starting with E0
	if (FlashSettings->XT83Keys && chunk[1] == 0xE0) {
		TR0 = 1; 
		return 1; // just pretend we sent it
	}
	
	TR0 = 0; //disable timer0  so send is not disabled while we're in the middle of buffer shuffling

	if (!ports[PORT_KEY].sendDisabled &&										 // send disabled by timer task, better not step on its toes
		((ports[PORT_KEY].sendBuffEnd + 1) & 0x3F) != ports[PORT_KEY].sendBuffStart) // not full
	{
		ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffEnd] = chunk;
		ports[PORT_KEY].sendBuffEnd = (ports[PORT_KEY].sendBuffEnd + 1) & 0x3F;
		TR0 = 1; // re-enable timer interrupt
		return 1;
	}

	TR0 = 1; // re-enable timer interrupt
	return 0;
}


void PressKey(uint8_t currchar)
{
    while (!SendKeyboard(
        FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Make[ASCIItoHID[currchar]] : HIDtoSET1_Make[ASCIItoHID[currchar]]))
        ;
}

void ReleaseKey(uint8_t currchar)
{
    while (!SendKeyboard(
        FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Break[ASCIItoHID[currchar]] : HIDtoSET1_Break[ASCIItoHID[currchar]]))
        ;
}

__xdata uint8_t GlobalSendBuff[8];

void PS2ProcessPort(uint8_t port)
{
	const uint8_t *chunk;
	uint8_t *chonk;
	static __data uint8_t sb;

	bool reEnter = 0;

	do
	{
		sb = ports[port].sendbit;
		reEnter = 0;

		// PS2 bit-bang state machine
		switch (ports[port].state)
		{
		// Poweron State, should probably be setting ports to default states, but poweron should have already done this
		case S_INIT: 
			ports[port].state = S_IDLE;
			reEnter = 1;
			break;

		// We're not (currently) sending anything or recieving anything, check if we need to
		case S_IDLE:

			// check to see if host is trying to inhibit (i.e. pulling clock low)
			if (!ReadPS2Clock(port))
			{
				// make sure data is high so we can detect it if it goes low
				WritePS2Data(port, 1);
				WritePS2Clock(port, 1);
				ports[port].state = S_PAUSE;
			}
			else
			{
				if (ports[port].rateLimit > 0) ports[port].rateLimit--;

				//if rateLimit is ok and buffer not empty
				//if buffer not empty
				if (ports[port].rateLimit == 0 && ports[port].sendBuffEnd != ports[port].sendBuffStart)
				{
					ports[port].rateLimit = PS2_RATE_LIMIT;

					sb = ports[port].sendbit = 0;
					ports[port].parity = 1;

					if (port == PORT_KEY)
					{
						chunk = ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffStart];
						ports[PORT_KEY].data = chunk[ports[PORT_KEY].bytenum + 1];
						//DEBUG_OUT("Consuming %x %x %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd, chunk[0], ports[port].data);
						ports[PORT_KEY].state = S_SEND_CLOCK_HIGH;
						//reEnter = 1;
					}
					else
					{ //mouse
						chonk = ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffStart];
						ports[PORT_MOUSE].data = chonk[ports[PORT_MOUSE].bytenum + 1];
						//DEBUG_OUT("Consuming %x %x %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd, chonk[0], ports[port].data);
									
						ports[PORT_MOUSE].state = S_SEND_CLOCK_HIGH;
						//reEnter = 1;
					}
				}
			}

			break;

		// clock is high and we are sending, so set next data bit
		case S_SEND_CLOCK_HIGH:

			// bit 0 is start bit (low)
			if (sb == 0)
			{
				WritePS2Data(port, 0);
			}

			// bits 1-8 are data bits
			else if (sb > 0 && sb < 9)
			{
				// set current bit data
				WritePS2Data(port, ports[port].data & 0x01);

				// calc parity and shift in preperation for next bit
				ports[port].parity = ports[port].parity ^ (ports[port].data & 0x01);
				ports[port].data = ports[port].data >> 1;
			}

			// bit 9 is parity
			else if (sb == 9)
			{
				WritePS2Data(port, ports[port].parity & 0x01);
			}

			// bit 10 is stop bit (high)
			else if (sb == 10)
			{
				WritePS2Data(port, 1);
			}

			ports[port].sendbit++;

			ports[port].state = S_SEND_CLOCK_FALL;

			break;

		// data has been set, send falling edge of clock
		case S_SEND_CLOCK_FALL:

			// if clock is already low, host is trying to inhibit
			if (!ReadPS2Clock(port))
			{
				// make sure clock/data are high so we can detect it if it goes low
				WritePS2Data(port, 1);
				WritePS2Clock(port, 1);

				// if interrupted before we've even sent the first bit then just pause, no need to resend current chunk
				if (sb == 1)
				{
					ports[port].sendbit--; // we will need to resend so go back one bit
					ports[port].state = S_MIDSEND_PAUSE;
				}
				// if interrupted halfway through byte, will need to send entire packet again
				else
				{
					ports[port].state = S_INHIBIT;
				}
			}
			else
			{
				WritePS2Clock(port, 0);

				ports[port].state = S_SEND_CLOCK_LOW;
			}

			break;

		// clock was already low when we tried to send it low, pause until it goes high again
		case S_MIDSEND_PAUSE:
			
			// I don't believe the entire chunk should be re-sent on a pause
			// uncomment if this is incorrect
			//ports[port].bytenum = 0;

			// wait for host to release clock
			if (ReadPS2Clock(port))
			{
				// if data line low then host wants to transmit
				if (!ReadPS2Data(port))
				{
					// go to full inhibit mode (to clear counters etc)
					ports[port].state = S_INHIBIT;
				}
				else
				{
					//otherwise, resent the bit and start again
					ports[port].state = S_SEND_CLOCK_HIGH;
				}
			}

			break;

		case S_SEND_CLOCK_LOW:
			ports[port].state = S_SEND_CLOCK_RISE;
			break;

		case S_SEND_CLOCK_RISE:
			//make clock high
			WritePS2Clock(port, 1);

			// if final bit, move onto next byte
			if (sb == 11)
			{
				ports[port].parity = 1;
				ports[port].sendbit = 0;

				// for keyboard get the next byte in the chunk
				if (port == PORT_KEY)
				{

					chunk = ports[port].sendBuff.chunky[ports[port].sendBuffStart];

					ports[port].bytenum++;

					// if we've run out of bytes in this chunk
					if (ports[port].bytenum == chunk[0])
					{
						// move onto next chunk
						//DEBUG_OUT("Consumed %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd);
						ports[port].sendBuffStart = (ports[port].sendBuffStart + 1) & 0x3F;
						ports[port].bytenum = 0;
					}
					else
					{
						ports[port].data = chunk[ports[port].bytenum + 1];
					}

					// give ourselves a little break between bytes
					ports[port].state = S_IDLE;
				}
				else /*if (port = PORT_MOUSE)*/
				{
					chonk = ports[port].sendBuff.chonky[ports[port].sendBuffStart];

					ports[port].bytenum++;

					// if we've run out of bytes in this chonk
					if (ports[port].bytenum == chonk[0])
					{
						// move onto next chonk
						//DEBUG_OUT("Consumed %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd);
						ports[port].sendBuffStart = (ports[port].sendBuffStart + 1) & 0x07;
						ports[port].bytenum = 0;
					}
					else
					{
						ports[port].data = chonk[ports[port].bytenum + 1];
					}

					// give ourselves a little break between bytes
					ports[port].state = S_IDLE;
				}
			}
			else
			{
				ports[port].state = S_SEND_CLOCK_HIGH;
			}

			break;

		case S_RECEIVE_CLOCK_HIGH:

			// bit 0 is start bit
			if (ports[port].recvbit == 0)
			{
				// do nothing, we've already checked it's ok (otherwise we wouldn't be in this state)
			}

			// bits 1-8 are data bits
			else if (ports[port].recvbit > 0 && ports[port].recvbit < 9)
			{
				ports[port].recvBuff |= (ReadPS2Data(port) << (ports[port].recvbit - 1));
				ports[port].parity = ports[port].parity ^ (ReadPS2Data(port) & 0x01);
			}

			// bit 9 is parity
			else if (ports[port].recvbit == 9)
			{
				if (ports[port].parity & 0x01 == ReadPS2Data(port))
				{
					// parity ok - reuse variable
					ports[port].parity = 1;
				}
				else
				{
					// abort if not
					ports[port].parity = 0;
				}
			}

			// bit 10 is stop bit (high)
			else if (ports[port].recvbit == 10)
			{
				// only accept data if stop bit is high and parity valid
				if (ReadPS2Data(port)) // && ports[port].parity) // lol it still isn't working
				{
					ports[port].recvout = ports[port].recvBuff;
				}

				ports[port].parity = 1;
				ports[port].recvbit = 0;

				// send ACK bit
				ports[port].state = S_RECEIVE_ACK_HIGH;

				reEnter = 1;
				break;
			}

			ports[port].recvbit++;

			ports[port].state = S_RECEIVE_CLOCK_FALL;
			break;

		case S_RECEIVE_CLOCK_FALL:
			WritePS2Clock(port, 0);
			ports[port].state = S_RECEIVE_CLOCK_LOW;
			break;

		case S_RECEIVE_CLOCK_LOW:
			ports[port].state = S_RECEIVE_CLOCK_RISE;
			break;

		case S_RECEIVE_CLOCK_RISE:
			WritePS2Clock(port, 1);
			ports[port].state = S_RECEIVE_CLOCK_HIGH;
			break;

		case S_RECEIVE_ACK_HIGH:
			// ACK bit is low
			WritePS2Data(port, 0);
			ports[port].state = S_RECEIVE_ACK_FALL;
			break;
		case S_RECEIVE_ACK_FALL:
			// Send it (make clock low)
			WritePS2Clock(port, 0);
			ports[port].state = S_RECEIVE_ACK_LOW;
			break;
		case S_RECEIVE_ACK_LOW:
			ports[port].state = S_RECEIVE_ACK_RISE;
			break;
		case S_RECEIVE_ACK_RISE:
			WritePS2Clock(port, 1);
			ports[port].state = S_RECEIVE_ACK_HIGH_DONE;
			break;
		case S_RECEIVE_ACK_HIGH_DONE:
			// release data
			WritePS2Data(port, 1);
			HandleReceived(port);
			ports[port].state = S_IDLE;
			break;

		// we're currently idle and host has put clock low
		// i.e. pause
		case S_PAUSE:

			// I don't believe the entire chunk should be re-sent on a pause
			// uncomment if this is incorrect
			// ports[port].bytenum = 0;

			// wait for host to release clock
			if (ReadPS2Clock(port))
			{
				// if data line low then host wants to transmit
				if (!ReadPS2Data(port))
				{
					// go to full inhibit mode (to clear counters etc)
					ports[port].state = S_INHIBIT;
				}
				else
				{
					//otherwise, just get on with it as normal
					ports[port].state = S_IDLE;
				}
			}

			break;

		case S_INHIBIT:

			// reset bit/byte indexes, as whole chunk will need to be re-sent if interrupted
			ports[port].sendbit = 0;
			/*if (port == PORT_KEY)*/ ports[port].bytenum = 0; // different devices disagree on whether this applies to mice or not...
			ports[port].parity = 1;

			// wait for host to release clock
			if (ReadPS2Clock(port))
			{
				// if data line low then host wants to transmit
				if (!ReadPS2Data(port))
				{
					ports[port].recvbit = 0;
					ports[port].recvBuff = 0;
					// empty send buffer
					ports[port].sendBuffStart = ports[port].sendBuffEnd;
					ports[port].sendDisabled = 1;
					ports[port].state = S_RECEIVE_CLOCK_HIGH;
				}
				else
				{
					//otherwise, restart sending current chunk
					ports[port].state = S_IDLE;
				}
			}

			break;

		case S_WAIT:

			// check to see if host is trying to inhibit (i.e. pulling clock low)
			/*if (!ReadPS2Clock(port)){
					// make sure data is high so we can detect it if it goes low
					WritePS2Data(port, 1);
					WritePS2Clock(port, 1);
					ports[port].state = S_INHIBIT;
					reEnter = 1;
					del=1;
				} else {

					del++;
					if (del > 100){
						del = 0;
						ports[port].state = S_IDLE;
					}
				}*/

			break;
		}
	} while (reEnter);
}