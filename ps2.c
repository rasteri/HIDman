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

__xdata ps2port keyboard = {
	S_INIT,	  //state
	PORT_KEY, //port
	0xFF,	  //data
	0,		  //sendbit
	0,		  //recvbit
	0,		  //parity
	0,		//sendingCustom

	0, //bytenum

	0, //recvvalid
	0, //recvout
	0, //recverror

	0, // lastByte

	0, //sendBuffStart
	0  //sendBuffEnd

};

__xdata ps2port mouse = {
	S_INIT,		//state
	PORT_MOUSE, //port
	0xFF,		//data
0,		  //sendbit
	0,		  //recvbit
	0,		  //parity
	0,		//sendingCustom

	0, //bytenum

	0, //recvvalid
	0, //recvout
	0, //recverror

	0, // lastByte

	0, //sendBuffStart
	0  //sendBuffEnd
};

void OutPort(unsigned char port, unsigned char channel, bool val)
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

bool GetPort(unsigned char port, unsigned char channel)
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

void SendPS2(ps2port *port, const uint8_t *chunk)
{
	// check for full
	if ((port->sendBuffEnd + 1) % 64 == port->sendBuffStart)
	{
		// do nothing
		DEBUG_OUT("Full\n");
	}
	else
	{
		port->sendBuff[port->sendBuffEnd] = chunk;
		port->sendBuffEnd = (port->sendBuffEnd + 1) % 64;
		//DEBUG_OUT("Produced %x %x\n", port->sendBuffStart, port->sendBuffEnd);
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
		if (msgbuffer[0] != keyboard.prevhid[0])
		{
			uint8_t rbits = msgbuffer[0];
			uint8_t pbits = keyboard.prevhid[0];

			// iterate through bits and compare to previous to see whats changed
			for (uint8_t j = 0; j < 8; j++)
			{

				if ((rbits & 0x01) != (pbits & 0x01))
				{

					if (rbits & 0x01)
					{
						SendPS2(&keyboard, ModtoPS2_MAKE[j]);
					}
					else
					{
						SendPS2(&keyboard, ModtoPS2_BREAK[j]);
					}
				}

				rbits = rbits >> 1;
				pbits = pbits >> 1;
			}

			keyboard.prevhid[0] = msgbuffer[0];
		}

		// iterate through all the HID bytes to see what's changed since last time
		for (uint8_t i = 2; i < 8; i++)
		{
			// key was pressed last time
			if (keyboard.prevhid[i])
			{

				// assume this will be a break code
				brk = 1;

				// see if this code is still present in current poll
				for (uint8_t j = 2; j < 8; j++)
				{
					if (keyboard.prevhid[i] == msgbuffer[j])
					{
						// if so, do not break
						brk = 0;
						break;
					}
				}

				if (brk)
				{
					DEBUG_OUT("Break %x\n", keyboard.prevhid[i]);
					// no break code for pause key, for some reason
					if (keyboard.prevhid[i] == 0x48)
						continue;

					repeat = 0;

					//send the break code
					if (keyboard.prevhid[i] <= 0x67)
						SendPS2(&keyboard, HIDtoPS2_Break[keyboard.prevhid[i]]);
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
					if (msgbuffer[i] == keyboard.prevhid[j])
					{
						// if so, no need to make
						make = false;
						break;
					}
				}

				if (make)
				{
					DEBUG_OUT("Make %x\n", msgbuffer[i]);
					repeat = msgbuffer[i];
					/*if (repeater)
						cancel_alarm(repeater);
					repeater = add_alarm_in_ms(delayms, repeat_callback, NULL, false);*/
					if (msgbuffer[i] <= 0x67)
						SendPS2(&keyboard, HIDtoPS2_Make[msgbuffer[i]]);
				}
			}

			keyboard.prevhid[i] = msgbuffer[i];
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



			
		}
		
		break;
	}
}

void PS2ProcessPort

void PS2BitBang(ps2port *port)
{
	const uint8_t *chunk;

	// get current chunk
	chunk = port->sendBuff[port->sendBuffStart];

	bool reEnter = 0;
	do
	{
		reEnter = 0;

		// PS2 bit-bang state machine
		switch (port->state)
		{
		case S_INIT:
			port->state = S_IDLE;
			reEnter = 1;
			break;

		case S_IDLE:
			// check to see if host is trying to inhibit (i.e. pulling clock low)
			if (!GetPort(port->port, CLOCK))
			{
				// make sure data is high so we can detect it if it goes low
				OutPort(port->port, DATA, 1);
				OutPort(port->port, CLOCK, 1);
				port->state = S_INHIBIT;
			}
			else
			{

				//if buffer not empty
				if (port->sendBuffEnd != port->sendBuffStart)
				{

					port->data = chunk[port->bytenum + 1];
					DEBUG_OUT("Consuming %x %x %x %x\n", port->sendBuffStart, port->sendBuffEnd, chunk[0], port->data);
					port->state = S_SEND_CLOCK_LOW;
					reEnter = 1;
				}
			}

			break;

		case S_SEND_CLOCK_LOW:
			// check to see if host is trying to inhibit (i.e. pulling clock low)
			if (!GetPort(port->port, CLOCK))
			{
				// make sure clock/data are high so we can detect it if it goes low
				OutPort(port->port, DATA, 1);
				OutPort(port->port, CLOCK, 1);

				// if interrupted before we've even sent the first bit then just pause, no need to resend current chunk
				if (port->sendbit == 0)
					port->state = S_PAUSE;
				// if interrupted halfway through byte, will need to send entire packet again
				else
					port->state = S_INHIBIT;
			}
			else
			{
				// bit 0 is start bit (low)
				if (port->sendbit == 0)
				{
					OutPort(port->port, DATA, 0);
				}

				// bits 1-8 are data bits
				else if (port->sendbit > 0 && port->sendbit < 9)
				{
					// set current bit data
					OutPort(port->port, DATA, port->data & 0x01);

					// calc parity and shift in preperation for next bit
					port->parity = port->parity ^ (port->data & 0x01);
					port->data = port->data >> 1;
				}

				// bit 9 is parity
				else if (port->sendbit == 9)
				{
					OutPort(port->port, DATA, port->parity & 0x01);
				}

				// bit 10 is stop bit (high)
				else if (port->sendbit == 10)
				{
					OutPort(port->port, DATA, 1);
				}

				// make clock low
				OutPort(port->port, CLOCK, 0);

				port->sendbit++;

				port->state = S_SEND_CLOCK_HIGH;
			}

			break;

		case S_SEND_CLOCK_HIGH:
			//make clock high
			OutPort(port->port, CLOCK, 1);

			// if final bit, move onto next byte
			if (port->sendbit == 11)
			{
				port->parity = 0;
				port->sendbit = 0;
				port->lastByte = port->data;
				port->bytenum++;

				// if we've run out of bytes in this chunk
				if (port->bytenum == chunk[0])
				{
					// move onto next chunk
					//DEBUG_OUT("Consumed %x %x\n", port->sendBuffStart, port->sendBuffEnd);
					port->sendBuffStart = (port->sendBuffStart + 1) % 64;
					port->bytenum = 0;
					port->state = S_IDLE;

					break;
				}
				else
				{
					port->data = chunk[port->bytenum + 1];
				}
			}

			port->state = S_SEND_CLOCK_LOW;

			break;

		case S_RECEIVE_CLOCK_LOW:
			OutPort(port->port, CLOCK, 0);
			port->state = S_RECEIVE_CLOCK_HIGH;
			break;

		case S_RECEIVE_CLOCK_HIGH:
			OutPort(port->port, CLOCK, 1);

			// bits 0-7 are data bits (start bit has already been done by this point)
			if (port->recvbit < 8)
			{
				port->recvBuff |= (GetPort(port->port, DATA) << port->recvbit);
				port->parity = port->parity ^ (GetPort(port->port, DATA) & 0x01);
			}

			// bit 8 is parity
			else if (port->recvbit == 8)
			{
				if (port->parity & 0x01 == GetPort(port->port, DATA))
				{
					// parity ok - reuse variable
					port->parity = 1;
				}
				else
				{
					// abort if not
					port->parity = 0;
				}
			}

			// bit 9 is stop bit (high)
			else if (port->recvbit == 9)
			{
				// only accept data if stop bit is high
				// should check parity too but it's not calcing properly lol
				if (GetPort(port->port, DATA))
				{
					port->recvout = port->recvBuff;
					port->recvvalid = 1;
				}

				// send ACK bit
				port->state = S_RECEIVE_ACK;
				break;
			}

			port->recvbit++;

			port->state = S_RECEIVE_CLOCK_LOW;
			break;

		case S_RECEIVE_ACK:
			// ACK bit is low
			OutPort(port->port, DATA, 0);
			// Send it (make clock low)
			OutPort(port->port, CLOCK, 0);

			// next time round clock will rise and we can go back to normal
			port->state = S_IDLE;
			break;

		case S_PAUSE:

			// wait for host to release clock
			if (GetPort(port->port, CLOCK))
			{
				// if data line low then host wants to transmit
				if (!GetPort(port->port, DATA))
				{
					// go to full inhibit mode (to clear counters etc)
					port->state = S_INHIBIT;
				}
				else
				{
					//otherwise, just get on with it as normal
					port->state = S_IDLE;
				}
			}

			break;

		case S_INHIBIT:
			// reset bit/byte indexes, as whole chunk will need to be re-sent if interrupted
			port->sendbit = 0;
			port->bytenum = 0;
			port->parity = 0;

			// wait for host to release clock
			if (GetPort(port->port, CLOCK))
			{
				// if data line low then host wants to transmit
				if (!GetPort(port->port, DATA))
				{
					port->recvbit = 0;
					port->recvBuff = 0;
					port->state = S_RECEIVE_CLOCK_LOW;
				}
				else
				{
					//otherwise, restart sending current chunk
					port->state = S_IDLE;
				}
			}

			break;

		case S_WAIT:

			// check to see if host is trying to inhibit (i.e. pulling clock low)
			/*if (!GetPort(port->port, CLOCK)){
					// make sure data is high so we can detect it if it goes low
					OutPort(port->port, DATA, 1);
					OutPort(port->port, CLOCK, 1);
					port->state = S_INHIBIT;
					reEnter = 1;
					P2 ^= 0b00001000;
					del=1;
				} else {

					del++;
					if (del > 100){
						del = 0;
						port->state = S_IDLE;
					}
				}*/

			break;
		}
	} while (reEnter);
}