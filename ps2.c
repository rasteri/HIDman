/*
	ps2.c
	
	Handles the low-level parts of the PS/2 protocol
	I.e. buffers and bit-bang state machine
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "protocol.h"

__xdata ps2port ports[] = {
	// keyboard
	{
		S_INIT, //state
		0xFF,	//data
		0,		//sendbit
		0x01,	//recvbit
		1,		//parity
		0,		//recvstate

		0, //bytenum

		0, //recvvalid
		0, //recvout
		0, //sendDisabled

		0, // lastByte

		0, //sendBuffStart
		0  //sendBuffEnd

	},

	//mouse
	{
		S_INIT, //state
		0xFF,	//data
		0,		//sendbit
		0x01,	//recvbit
		1,		//parity
		0,		//recvstate

		0, //bytenum

		0, //recvvalid
		0, //recvout
		0, //sendDisabled

		0, // lastByte

		0, //sendBuffStart
		0  //sendBuffEnd
	}

};

bool GetPort(uint8_t port, uint8_t channel)
{
	if (port == PORT_KEY)
		if (channel == CLOCK)
			return KEY_CLOCK;
		else
			return KEY_DATA;

	else if (port == PORT_MOUSE)
		if (channel == CLOCK)
			return MOUSE_CLOCK;
		else
			return MOUSE_DATA;
}

void SendKeyboard(const uint8_t *chunk)
{

	TR0 = 0; //disable timer0  so send is not disabled while we're in the middle of buffer shuffling

	if (!ports[PORT_KEY].sendDisabled &&										 // send disabled by timer task, better not step on its toes
		chunk != NULL &&														 // chunk is valid
		(ports[PORT_KEY].sendBuffEnd + 1) % 64 != ports[PORT_KEY].sendBuffStart) // not full
	{
		ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffEnd] = chunk;
		ports[PORT_KEY].sendBuffEnd = (ports[PORT_KEY].sendBuffEnd + 1) % 64;
	}

	TR0 = 1; // re-enable timer interrupt
}

void SendMouse(uint8_t byte)
{
	TR0 = 0; //disable timer0  so send is not disabled while we're in the middle of buffer shuffling

	if (!ports[PORT_MOUSE].sendDisabled &&											 // send disabled by timer task, better not step on its toes
		(ports[PORT_MOUSE].sendBuffEnd + 1) % 64 != ports[PORT_MOUSE].sendBuffStart) // not full
	{
		ports[PORT_MOUSE].sendBuff.arbitrary[ports[PORT_MOUSE].sendBuffEnd] = byte;
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 64;
	}

	TR0 = 1; // re-enable timer interrupt
}

void PS2ProcessPort(uint8_t port)
{
	const uint8_t *chunk;
	__data uint8_t sb;

	bool reEnter = 0;
	do
	{
		sb = ports[port].sendbit;
		reEnter = 0;

		// PS2 bit-bang state machine
		switch (ports[port].state)
		{
		case S_INIT:
			ports[port].state = S_IDLE;
			reEnter = 1;
			break;

		case S_IDLE:
			// check to see if host is trying to inhibit (i.e. pulling clock low)
			if (!GetPort(port, CLOCK))
			{
				// make sure data is high so we can detect it if it goes low
				OutPort(port, DATA, 1);
				OutPort(port, CLOCK, 1);
				ports[port].state = S_PAUSE;
			}
			else
			{

				//if buffer not empty
				if (ports[port].sendBuffEnd != ports[port].sendBuffStart)
				{
					if (port == PORT_KEY)
					{
						chunk = ports[port].sendBuff.chunky[ports[port].sendBuffStart];
						ports[port].data = chunk[ports[port].bytenum + 1];
						//DEBUG_OUT("Consuming %x %x %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd, chunk[0], ports[port].data);
						ports[port].state = S_SEND_CLOCK_HIGH;
						//reEnter = 1;
					}
					else
					{ //mouse
						ports[port].data = ports[port].sendBuff.arbitrary[ports[port].sendBuffStart];
						ports[port].state = S_SEND_CLOCK_HIGH;
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
				OutPort(port, DATA, 0);
			}

			// bits 1-8 are data bits
			else if (sb > 0 && sb < 9)
			{
				// set current bit data
				OutPort(port, DATA, ports[port].data & 0x01);

				// calc parity and shift in preperation for next bit
				ports[port].parity = ports[port].parity ^ (ports[port].data & 0x01);
				ports[port].data = ports[port].data >> 1;
			}

			// bit 9 is parity
			else if (sb == 9)
			{
				OutPort(port, DATA, ports[port].parity & 0x01);
			}

			// bit 10 is stop bit (high)
			else if (sb == 10)
			{
				OutPort(port, DATA, 1);
			}

			ports[port].sendbit++;

			ports[port].state = S_SEND_CLOCK_FALL;

			break;

		// data has been set, send falling edge of clock
		case S_SEND_CLOCK_FALL:

			// if clock is already low, host is trying to inhibit
			if (!GetPort(port, CLOCK))
			{
				// make sure clock/data are high so we can detect it if it goes low
				OutPort(port, DATA, 1);
				OutPort(port, CLOCK, 1);

				// if interrupted before we've even sent the first bit then just pause, no need to resend current chunk
				if (sb == 1)
					ports[port].state = S_PAUSE;
				// if interrupted halfway through byte, will need to send entire packet again
				else
				{
					ports[port].state = S_INHIBIT;
				}
			}
			else
			{
				OutPort(port, CLOCK, 0);
				ports[port].state = S_SEND_CLOCK_LOW;
			}

			break;

		case S_SEND_CLOCK_LOW:
			ports[port].state = S_SEND_CLOCK_RISE;
			break;

		case S_SEND_CLOCK_RISE:
			//make clock high
			OutPort(port, CLOCK, 1);

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
						ports[port].sendBuffStart = (ports[port].sendBuffStart + 1) % 64;
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
					// move onto next byte
					ports[port].sendBuffStart = (ports[port].sendBuffStart + 1) % 64;
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
				ports[port].recvBuff |= (GetPort(port, DATA) << (ports[port].recvbit - 1));
				ports[port].parity = ports[port].parity ^ (GetPort(port, DATA) & 0x01);
			}

			// bit 9 is parity
			else if (ports[port].recvbit == 9)
			{
				if (ports[port].parity & 0x01 == GetPort(port, DATA))
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
				if (GetPort(port, DATA)) // && ports[port].parity) // lol it still isn't working
				{
					ports[port].recvout = ports[port].recvBuff;
					ports[port].recvvalid = 1;
					//

					//delayUs(20);
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
			OutPort(port, CLOCK, 0);
			ports[port].state = S_RECEIVE_CLOCK_LOW;
			break;

		case S_RECEIVE_CLOCK_LOW:
			ports[port].state = S_RECEIVE_CLOCK_RISE;
			break;

		case S_RECEIVE_CLOCK_RISE:
			OutPort(port, CLOCK, 1);
			ports[port].state = S_RECEIVE_CLOCK_HIGH;
			break;

		case S_RECEIVE_ACK_HIGH:
			// ACK bit is low
			OutPort(port, DATA, 0);
			ports[port].state = S_RECEIVE_ACK_FALL;
			break;
		case S_RECEIVE_ACK_FALL:
			// Send it (make clock low)
			OutPort(port, CLOCK, 0);
			ports[port].state = S_RECEIVE_ACK_LOW;
			break;
		case S_RECEIVE_ACK_LOW:
			ports[port].state = S_RECEIVE_ACK_RISE;
			break;
		case S_RECEIVE_ACK_RISE:
			OutPort(port, CLOCK, 1);
			ports[port].state = S_RECEIVE_ACK_HIGH_DONE;
			break;
		case S_RECEIVE_ACK_HIGH_DONE:
			// release data
			OutPort(port, DATA, 1);
			HandleReceived(port);
			ports[port].state = S_IDLE;
			break;

		case S_PAUSE:

			// wait for host to release clock
			if (GetPort(port, CLOCK))
			{
				// if data line low then host wants to transmit
				if (!GetPort(port, DATA))
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
			ports[port].bytenum = 0;
			ports[port].parity = 1;

			// wait for host to release clock
			if (GetPort(port, CLOCK))
			{
				// if data line low then host wants to transmit
				if (!GetPort(port, DATA))
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
			/*if (!GetPort(port, CLOCK)){
					// make sure data is high so we can detect it if it goes low
					OutPort(port, DATA, 1);
					OutPort(port, CLOCK, 1);
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