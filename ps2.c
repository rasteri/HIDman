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

SBIT(KEY_CLOCK, 0xA0, 0); // port 2.0
SBIT(KEY_DATA, 0xA0, 1);  // port 2.1

SBIT(MOUSE_CLOCK, 0xA0, 2); // port 2.2
SBIT(MOUSE_DATA, 0xA0, 3);	// port 2.3

__xdata ps2port ports[] = {
	// keyboard
	{
		S_INIT,	  //state
		0xFF,	  //data
		0,		  //sendbit
		0x01,	  //recvbit
		1,		  //parity
		0,		  //recvstate

		0, //bytenum

		0, //recvvalid
		0, //recvout
		0, //recverror

		0, // lastByte

		0, //sendBuffStart
		0  //sendBuffEnd

	},

	//mouse
	{
	S_INIT,	  //state
	0xFF,	  //data
	0,		  //sendbit
	0x01,	  //recvbit
	1,		  //parity
	0,		  //recvstate

	0, //bytenum

	0, //recvvalid
	0, //recvout
	0, //recverror

	0, // lastByte

	0, //sendBuffStart
	0  //sendBuffEnd
}
	
};

/*
void OutPort2(unsigned char port, unsigned char channel, bool val)
{
	if (port == PORT_KEY)
		if (channel == CLOCK)
			KEY_CLOCK = val;
		else
			KEY_DATA = val;
	else if (port == PORT_MOUSE)
		if (channel == CLOCK)
			MOUSE_CLOCK = val;
		else
			MOUSE_DATA = val;
}
*/

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

void SendPS2(uint8_t port, const uint8_t *chunk)
{
	// check for full
	if ((ports[port].sendBuffEnd + 1) % 64 == ports[port].sendBuffStart)
	{
		// do nothing
		//DEBUG_OUT("Full\n");
	}
	else
	{
		ports[port].sendBuff.chunky[ports[port].sendBuffEnd] = chunk;
		ports[port].sendBuffEnd = (ports[port].sendBuffEnd + 1) % 64;
		//DEBUG_OUT("Produced %x %x\n", ports[port].sendBuffStart, ports[port].sendBuffEnd);
	}
}

bool repeat;

void SendHIDPS2(unsigned short length, unsigned char type, unsigned char __xdata *msgbuffer)
{
	bool brk = 0, make = 0;
	switch (type)
	{
	case REPORT_USAGE_KEYBOARD:

		// do special keys first
		if (msgbuffer[0] != ports[PORT_KEY].prevhid[0])
		{
			uint8_t rbits = msgbuffer[0];
			uint8_t pbits = ports[PORT_KEY].prevhid[0];

			// iterate through bits and compare to previous to see whats changed
			for (uint8_t j = 0; j < 8; j++)
			{

				if ((rbits & 0x01) != (pbits & 0x01))
				{

					if (rbits & 0x01)
					{
						SendPS2(PORT_KEY, ModtoPS2_MAKE[j]);
					}
					else
					{
						SendPS2(PORT_KEY, ModtoPS2_BREAK[j]);
					}
				}

				rbits = rbits >> 1;
				pbits = pbits >> 1;
			}

			ports[PORT_KEY].prevhid[0] = msgbuffer[0];
		}

		// iterate through all the HID bytes to see what's changed since last time
		for (uint8_t i = 2; i < 8; i++)
		{
			// key was pressed last time
			if (ports[PORT_KEY].prevhid[i])
			{

				// assume this will be a break code
				brk = 1;

				// see if this code is still present in current poll
				for (uint8_t j = 2; j < 8; j++)
				{
					if (ports[PORT_KEY].prevhid[i] == msgbuffer[j])
					{
						// if so, do not break
						brk = 0;
						break;
					}
				}

				if (brk)
				{
					//DEBUG_OUT("Break %x\n", keyboard.prevhid[i]);
					// no break code for pause key, for some reason
					if (ports[PORT_KEY].prevhid[i] == 0x48)
						continue;

					repeat = 0;

					//send the break code
					if (ports[PORT_KEY].prevhid[i] <= 0x67)
						SendPS2(PORT_KEY, HIDtoPS2_Break[ports[PORT_KEY].prevhid[i]]);
				}
			}

			// key is pressed this time
			if (msgbuffer[i])
			{
				// assume we need to make
				make = true;

				// see if key was present in previous poll
				for (uint8_t j = 2; j < 8; j++)
				{
					if (msgbuffer[i] == ports[PORT_KEY].prevhid[j])
					{
						// if so, no need to make
						make = false;
						break;
					}
				}

				if (make)
				{
					//DEBUG_OUT("Make %x\n", msgbuffer[i]);
					repeat = msgbuffer[i];
					/*if (repeater)
						cancel_alarm(repeater);
					repeater = add_alarm_in_ms(delayms, repeat_callback, NULL, false);*/
					if (msgbuffer[i] <= 0x67)
						SendPS2(PORT_KEY, HIDtoPS2_Make[msgbuffer[i]]);
				}
			}

			ports[PORT_KEY].prevhid[i] = msgbuffer[i];
		}
		break;

	case REPORT_USAGE_MOUSE:
		DEBUG_OUT("Mouse : ");
		for (int p = 0; p < length; p++)
			DEBUG_OUT("%x ", msgbuffer[p]);
		DEBUG_OUT("\n");

		//byte 0 appears to always be 1
		//byte 1 is buttons
		//byte 2 is x movement (8 bit signed)
		//byte 3 is y movement (8 bit signed)

		break;
	}
}

inline void HandleReceived(uint8_t port)
{
	if (port == PORT_KEY)
	{
		switch (ports[port].recvstate)
		{
		case R_IDLE:

			switch (ports[port].recvout)
			{
			case 0xFF:
				SendPS2(port, KEY_ACK);
				SendPS2(port, KEY_BATCOMPLETE);
				break;

			// set LEDs
			case 0xED:
				SendPS2(port, KEY_ACK);
				ports[port].recvstate = R_LEDS;
				break;

			// set repeat
			case 0xF3:
				SendPS2(port, KEY_ACK);
				ports[port].recvstate = R_REPEAT;
				break;

			// ID
			case 0xF2:
				SendPS2(port, KEY_ACK);
				SendPS2(port, KEY_ID);
				break;

			// Enable
			case 0xF4:
				SendPS2(port, KEY_ACK);
				break;

			// Disable
			case 0xF5:
				SendPS2(port, KEY_ACK);
				break;
			}

			break;

		case R_LEDS:
			// TODO blinkenlights
			ports[port].recvstate = R_IDLE;
			SendPS2(port, KEY_ACK);
			break;

		case R_REPEAT:
			// TODO repeat
			ports[port].recvstate = R_IDLE;
			SendPS2(port, KEY_ACK);
			break;
		}
	}

	else if (port == PORT_MOUSE)
	{
		switch (ports[port].recvstate)
		{
		case R_IDLE:
			switch (ports[port].recvout)
			{
			case 0xFF:
				SendPS2(port, MOUSE_ACK);
				SendPS2(port, MOUSE_BATCOMPLETE);
				break;

			// ID
			case 0xF2:
				SendPS2(port, MOUSE_ACK);
				SendPS2(port, MOUSE_ID);
				break;

			//codes that will get a second byte
			case 0xE8: // Set resolution
			case 0xF3:
				SendPS2(port, MOUSE_ACK);
				ports[port].recvstate = R_SECONDBYTE;
				break;
			}
			break;
		}
	}
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
				ports[port].state = S_INHIBIT;
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
						ports[port].state = S_SEND_CLOCK_LOW;
						P2=P2 ^ 0b10000;
						//reEnter = 1;
					}
					else
					{ //mouse
						ports[port].data = ports[port].sendBuff.arbitrary[ports[port].sendBuffStart];
						ports[port].state = S_SEND_CLOCK_LOW;
						//reEnter = 1;
					}
				}
			}

			break;

		case S_SEND_CLOCK_LOW:

			// check to see if host is trying to inhibit (i.e. pulling clock low)
			if (!GetPort(port, CLOCK))
			{
				// make sure clock/data are high so we can detect it if it goes low
				OutPort(port, DATA, 1);
				OutPort(port, CLOCK, 1);

				// if interrupted before we've even sent the first bit then just pause, no need to resend current chunk
				if (sb == 0)
					ports[port].state = S_PAUSE;
				// if interrupted halfway through byte, will need to send entire packet again
				else
					ports[port].state = S_INHIBIT;
			}
			else
			{
				
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

				// make clock low
				OutPort(port, CLOCK, 0);

				ports[port].sendbit++;

				ports[port].state = S_SEND_CLOCK_HIGH;
			}

			break;

		case S_SEND_CLOCK_HIGH:
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
				ports[port].state = S_SEND_CLOCK_LOW;
			}

			break;

		case S_RECEIVE_CLOCK_LOW:
			OutPort(port, CLOCK, 0);
			ports[port].state = S_RECEIVE_CLOCK_HIGH;
			break;

		case S_RECEIVE_CLOCK_HIGH:
			OutPort(port, CLOCK, 1);

			// bits 0-7 are data bits (start bit has already been done by this point)
			if (ports[port].recvbit < 8)
			{
				ports[port].recvBuff |= (GetPort(port, DATA) << ports[port].recvbit);
				ports[port].parity = ports[port].parity ^ (GetPort(port, DATA) & 0x01);
			}

			// bit 8 is parity
			else if (ports[port].recvbit == 8)
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

			// bit 9 is stop bit (high)
			else if (ports[port].recvbit == 9)
			{
				// only accept data if stop bit is high and parity valid
				if (GetPort(port, DATA)) // && ports[port].parity) // lol it still isn't working
				{
					ports[port].recvout = ports[port].recvBuff;
					ports[port].recvvalid = 1;
					HandleReceived(port);
				}

				ports[port].parity = 1;
				ports[port].recvbit = 0;

				// send ACK bit
				ports[port].state = S_RECEIVE_ACK;
				break;
			}

			ports[port].recvbit++;

			ports[port].state = S_RECEIVE_CLOCK_LOW;
			break;

		case S_RECEIVE_ACK:
			// ACK bit is low
			OutPort(port, DATA, 0);
			// Send it (make clock low)
			OutPort(port, CLOCK, 0);

			// next time round clock will rise and we can go back to normal
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
					ports[port].state = S_RECEIVE_CLOCK_LOW;
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
					P2 ^= 0b00001000;
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