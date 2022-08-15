#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "ps2.h"

SBIT(KEY_CLOCK, 0xA0, 0); // port 2.0
SBIT(KEY_DATA, 0xA0, 1);  // port 2.1

SBIT(MOUSE_CLOCK, 0xA0, 2); // port 2.2
SBIT(MOUSE_DATA, 0xA0, 3);	// port 2.3

const uint8_t KEYA_MAKE[] = {1, 0x1C};
const uint8_t KEYB_MAKE[] = {1, 0x32};
const uint8_t KEYC_MAKE[] = {1, 0x21};
const uint8_t KEYD_MAKE[] = {1, 0x23};
const uint8_t KEYE_MAKE[] = {1, 0x24};
const uint8_t KEYF_MAKE[] = {1, 0x2B};
const uint8_t KEYG_MAKE[] = {1, 0x34};
const uint8_t KEYH_MAKE[] = {1, 0x33};
const uint8_t KEYI_MAKE[] = {1, 0x43};
const uint8_t KEYJ_MAKE[] = {1, 0x3B};
const uint8_t KEYK_MAKE[] = {1, 0x42};
const uint8_t KEYL_MAKE[] = {1, 0x4B};
const uint8_t KEYM_MAKE[] = {1, 0x3A};
const uint8_t KEYN_MAKE[] = {1, 0x31};
const uint8_t KEYO_MAKE[] = {1, 0x44};
const uint8_t KEYP_MAKE[] = {1, 0x4D};
const uint8_t KEYQ_MAKE[] = {1, 0x15};
const uint8_t KEYR_MAKE[] = {1, 0x2D};
const uint8_t KEYS_MAKE[] = {1, 0x1B};
const uint8_t KEYT_MAKE[] = {1, 0x2C};
const uint8_t KEYU_MAKE[] = {1, 0x3C};
const uint8_t KEYV_MAKE[] = {1, 0x2A};
const uint8_t KEYW_MAKE[] = {1, 0x1D};
const uint8_t KEYX_MAKE[] = {1, 0x22};
const uint8_t KEYY_MAKE[] = {1, 0x35};
const uint8_t KEYZ_MAKE[] = {1, 0x1A};

const uint8_t KEYA_BREAK[] = {2, 0xF0, 0x1C};
const uint8_t KEYB_BREAK[] = {2, 0xF0, 0x32};
const uint8_t KEYC_BREAK[] = {2, 0xF0, 0x21};
const uint8_t KEYD_BREAK[] = {2, 0xF0, 0x23};
const uint8_t KEYE_BREAK[] = {2, 0xF0, 0x24};
const uint8_t KEYF_BREAK[] = {2, 0xF0, 0x2B};
const uint8_t KEYG_BREAK[] = {2, 0xF0, 0x34};
const uint8_t KEYH_BREAK[] = {2, 0xF0, 0x33};
const uint8_t KEYI_BREAK[] = {2, 0xF0, 0x43};
const uint8_t KEYJ_BREAK[] = {2, 0xF0, 0x3B};
const uint8_t KEYK_BREAK[] = {2, 0xF0, 0x42};
const uint8_t KEYL_BREAK[] = {2, 0xF0, 0x4B};
const uint8_t KEYM_BREAK[] = {2, 0xF0, 0x3A};
const uint8_t KEYN_BREAK[] = {2, 0xF0, 0x31};
const uint8_t KEYO_BREAK[] = {2, 0xF0, 0x44};
const uint8_t KEYP_BREAK[] = {2, 0xF0, 0x4D};
const uint8_t KEYQ_BREAK[] = {2, 0xF0, 0x15};
const uint8_t KEYR_BREAK[] = {2, 0xF0, 0x2D};
const uint8_t KEYS_BREAK[] = {2, 0xF0, 0x1B};
const uint8_t KEYT_BREAK[] = {2, 0xF0, 0x2C};
const uint8_t KEYU_BREAK[] = {2, 0xF0, 0x3C};
const uint8_t KEYV_BREAK[] = {2, 0xF0, 0x2A};
const uint8_t KEYW_BREAK[] = {2, 0xF0, 0x1D};
const uint8_t KEYX_BREAK[] = {2, 0xF0, 0x22};
const uint8_t KEYY_BREAK[] = {2, 0xF0, 0x35};
const uint8_t KEYZ_BREAK[] = {2, 0xF0, 0x1A};

const uint8_t *HIDtoPS2_Make[] =
	{
		NULL, NULL, NULL, NULL,
		KEYA_MAKE,
		KEYB_MAKE,
		KEYC_MAKE,
		KEYD_MAKE,
		KEYE_MAKE,
		KEYF_MAKE,
		KEYG_MAKE,
		KEYH_MAKE,
		KEYI_MAKE,
		KEYJ_MAKE,
		KEYK_MAKE,
		KEYL_MAKE,
		KEYM_MAKE,
		KEYN_MAKE,
		KEYO_MAKE,
		KEYP_MAKE,
		KEYQ_MAKE,
		KEYR_MAKE,
		KEYS_MAKE,
		KEYT_MAKE,
		KEYU_MAKE,
		KEYV_MAKE,
		KEYW_MAKE,
		KEYX_MAKE,
		KEYY_MAKE,
		KEYZ_MAKE};

const uint8_t *HIDtoPS2_Break[] =
	{
		NULL, NULL, NULL, NULL,
		KEYA_BREAK,
		KEYB_BREAK,
		KEYC_BREAK,
		KEYD_BREAK,
		KEYE_BREAK,
		KEYF_BREAK,
		KEYG_BREAK,
		KEYH_BREAK,
		KEYI_BREAK,
		KEYJ_BREAK,
		KEYK_BREAK,
		KEYL_BREAK,
		KEYM_BREAK,
		KEYN_BREAK,
		KEYO_BREAK,
		KEYP_BREAK,
		KEYQ_BREAK,
		KEYR_BREAK,
		KEYS_BREAK,
		KEYT_BREAK,
		KEYU_BREAK,
		KEYV_BREAK,
		KEYW_BREAK,
		KEYX_BREAK,
		KEYY_BREAK,
		KEYZ_BREAK};

__xdata ps2port keyboard = {
	S_INIT,	  //state
	PORT_KEY, //port
	0xFF,	  //data
	0,		  //sendbit
	0,		  //recvbit
	0,		  //parity

	0, //bytenum

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

	else if (channel == CLOCK)
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

	else if (channel == CLOCK)
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
	}
	else
	{
		port->sendBuff[port->sendBuffEnd] = chunk;
		port->sendBuffEnd = (port->sendBuffEnd + 1) % 64;
	}
}

bool repeat;

void SendHIDPS2(unsigned short length, unsigned char type, unsigned char __xdata *msgbuffer)
{
	bool brk = 0, make = 0;
	switch (type)
	{
	case REPORT_USAGE_KEYBOARD:

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
					// no break code for pause key, for some reason
					if (keyboard.prevhid[i] == 0x48)
						continue;

					repeat = 0;

					//send the break code
					SendPS2(&keyboard, HIDtoPS2_Break[keyboard.prevhid[i]]);
					P2 ^= 0b00001000;
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

					repeat = msgbuffer[i];
					/*if (repeater)
						cancel_alarm(repeater);
					repeater = add_alarm_in_ms(delayms, repeat_callback, NULL, false);*/

					SendPS2(&keyboard, HIDtoPS2_Make[msgbuffer[i]]);
				}
			}

			keyboard.prevhid[i] = msgbuffer[i];
		}
		break;
	}

	
}

void PS2ProcessPort(ps2port *port)
{
	uint8_t *chunk;

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

				port->bytenum++;

				// if we've run out of bytes in this chunk
				if (port->bytenum == chunk[0])
				{
					// move onto next chunk
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
			port->recvbit++;

			if (port->recvbit == 11)
			{
				// send ACK bit
				port->state = S_RECEIVE_ACK;
				break;
			}

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

		case S_INHIBIT:
			// reset bit/byte indexes, as whole chunk will need to be re-sent if interrupted
			port->sendbit = 0;
			port->bytenum = 0;

			// wait for host to release clock
			if (GetPort(port->port, CLOCK))
			{
				// if data line low then host wants to transmit
				if (!GetPort(port->port, DATA))
				{
					port->state = S_RECEIVE_CLOCK_LOW;
					port->recvbit = 0;
					port->recvBuff = 0;
				}
				else
				{
					//otherwise, just get on with it as normal
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