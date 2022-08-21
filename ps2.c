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

SBIT(KEY_CLOCK, 0xA0, 0);
SBIT(KEY_DATA, 0xA0, 1);

SBIT(MOUSE_CLOCK, 0xA0, 2);
SBIT(MOUSE_DATA, 0xA0, 3);

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
		0, //recverror

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

void SendKeyboard(const uint8_t *chunk)
{
	// check for full
	if ((ports[PORT_KEY].sendBuffEnd + 1) % 64 == ports[PORT_KEY].sendBuffStart)
	{
		// do nothing
		//DEBUG_OUT("Full\n");
	}
	else
	{
		ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffEnd] = chunk;
		ports[PORT_KEY].sendBuffEnd = (ports[PORT_KEY].sendBuffEnd + 1) % 64;
		//DEBUG_OUT("Produced %x %x\n", ports[PORT_KEY].sendBuffStart, ports[PORT_KEY].sendBuffEnd);
	}
}

void SendMouse(uint8_t byte)
{
	// check for full
	if ((ports[PORT_MOUSE].sendBuffEnd + 1) % 64 == ports[PORT_MOUSE].sendBuffStart)
	{
		// do nothing
		//DEBUG_OUT("Full\n");
	}
	else
	{
		ports[PORT_MOUSE].sendBuff.arbitrary[ports[PORT_MOUSE].sendBuffEnd] = byte;
		ports[PORT_MOUSE].sendBuffEnd = (ports[PORT_MOUSE].sendBuffEnd + 1) % 64;
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
						SendKeyboard(ModtoPS2_MAKE[j]);
					}
					else
					{
						SendKeyboard(ModtoPS2_BREAK[j]);
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
						SendKeyboard(HIDtoPS2_Break[ports[PORT_KEY].prevhid[i]]);
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
						SendKeyboard(HIDtoPS2_Make[msgbuffer[i]]);
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

		//HID :
		//byte 0 appears to always be 1
		//byte 1 is buttons
		//byte 2 is x movement (8 bit signed)
		//byte 3 is y movement (8 bit signed)

		signed char x = (signed char)msgbuffer[2], y = (signed char)msgbuffer[3];

		// PS2 mice have the y-axis inverted from HID
		y = -y;

		// First PS2 byte
		// bit3 always set
		uint8_t tmp = 0x08;

		// bottom 3 bits of button format is the same
		tmp |= msgbuffer[1] & 0x07;

		// X sign
		tmp |= ((x & 0x80) >> 3);

		// Y sign
		tmp |= ((y & 0x80) >> 2);

		SendMouse(tmp);

		//Second PS2 byte (X movement)
		SendMouse((uint8_t)x);

		//Third PS2 byte (Y movement)
		SendMouse((uint8_t)y);

		break;

	case 0x04: //joystick

		if (msgbuffer[0] == 1)
		{
			// iterate through all the HID bytes to see what's changed since last time
			for (uint8_t i = 1; i < 8; i++)
			{
				if (ports[PORT_KEY].prevhid[i] != msgbuffer[i])
				{
					switch (i)
					{
					case 1: // right analog stick Y
						break;
					case 2: // right analog stick X
						break;
					case 3: // left analog stick X
						// breaks
						if (msgbuffer[3] >= 64 && ports[PORT_KEY].prevhid[3] < 64) // not left but left last time
							SendKeyboard(KEY_LEFT_BREAK);
						if (msgbuffer[3] <= 192 && ports[PORT_KEY].prevhid[3] > 192) // not right but right last time
							SendKeyboard(KEY_RIGHT_BREAK);
						// makes
						if (msgbuffer[3] < 64 && ports[PORT_KEY].prevhid[3] >= 64) // left and not left last time
							SendKeyboard(KEY_LEFT_MAKE);
						if (msgbuffer[3] > 192 && ports[PORT_KEY].prevhid[3] <= 192) // right and not right last time
							SendKeyboard(KEY_RIGHT_MAKE);

						break;
					case 4: // left analog stick Y
						// breaks
						if (msgbuffer[4] >= 64 && ports[PORT_KEY].prevhid[4] < 64) // not up but up last time
							SendKeyboard(KEY_UP_BREAK);
						if (msgbuffer[4] <= 192 && ports[PORT_KEY].prevhid[4] > 192) // not down but down last time
							SendKeyboard(KEY_DOWN_BREAK);
						// makes
						if (msgbuffer[4] < 64 && ports[PORT_KEY].prevhid[4] >= 64) // up and not up last time
							SendKeyboard(KEY_UP_MAKE);
						if (msgbuffer[4] > 192 && ports[PORT_KEY].prevhid[4] <= 192) // down and not down last time
							SendKeyboard(KEY_DOWN_MAKE);
						break;
					case 5:
						// breaks
						if (msgbuffer[5] & 0x10 && !(ports[PORT_KEY].prevhid[5] & 0x10))
							SendKeyboard(KEY_LCTRL_MAKE);
						if (msgbuffer[5] & 0x20 && !(ports[PORT_KEY].prevhid[5] & 0x20))
							SendKeyboard(KEY_SPACE_MAKE);
						if (msgbuffer[5] & 0x40 && !(ports[PORT_KEY].prevhid[5] & 0x40))
							SendKeyboard(KEY_LALT_MAKE);
						if (msgbuffer[5] & 0x80 && !(ports[PORT_KEY].prevhid[5] & 0x80))
							SendKeyboard(KEY_ENTER_MAKE);
						// makes
						if (!(msgbuffer[5] & 0x10) && ports[PORT_KEY].prevhid[5] & 0x10)
							SendKeyboard(KEY_LCTRL_BREAK);
						if (!(msgbuffer[5] & 0x20) && ports[PORT_KEY].prevhid[5] & 0x20)
							SendKeyboard(KEY_SPACE_BREAK);
						if (!(msgbuffer[5] & 0x40) && ports[PORT_KEY].prevhid[5] & 0x40)
							SendKeyboard(KEY_LALT_BREAK);
						if (!(msgbuffer[5] & 0x80) && ports[PORT_KEY].prevhid[5] & 0x80)
							SendKeyboard(KEY_ENTER_BREAK);
						break;
					case 6:
					break;
					}
				}

				ports[PORT_KEY].prevhid[i] = msgbuffer[i];
			}
		}
		break;

	default:
		// byte 0 is the number of controller
		// byte 1/2 are right analog stick Y/X
		// byte 3/4 are left analog stick/DPAD X/Y
		// byte 5 -
		//   bits 0-3 are always 1
		//   bits 4-7 are thumb buttons
		// byte 6 -
		//   bits 0-3 are shoulder buttons
		//   bits 4-7 are always 0

		if (msgbuffer[0] == 1)
		{
			ANDYS_DEBUG_OUT("dunno %x : ", type);
			for (int p = 0; p < length; p++)
				ANDYS_DEBUG_OUT("%x ", msgbuffer[p]);
			ANDYS_DEBUG_OUT("\n");
			break;
		}
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
				SendKeyboard(KEY_ACK);
				SendKeyboard(KEY_BATCOMPLETE);
				break;

			// set LEDs
			case 0xED:
				SendKeyboard(KEY_ACK);
				ports[port].recvstate = R_LEDS;
				break;

			// set repeat
			case 0xF3:
				SendKeyboard(KEY_ACK);
				ports[port].recvstate = R_REPEAT;
				break;

			// ID
			case 0xF2:
				SendKeyboard(KEY_ACK);
				SendKeyboard(KEY_ID);
				break;

			// Enable
			case 0xF4:
				SendKeyboard(KEY_ACK);
				break;

			// Disable
			case 0xF5:
				SendKeyboard(KEY_ACK);
				break;
			}

			break;

		case R_LEDS:
			// TODO blinkenlights
			ports[port].recvstate = R_IDLE;
			SendKeyboard(KEY_ACK);
			break;

		case R_REPEAT:
			// TODO repeat
			ports[port].recvstate = R_IDLE;
			SendKeyboard(KEY_ACK);
			break;
		}
	}

	/*
__code uint8_t MOUSE_ACK[] = {1, 0xFA};
__code uint8_t MOUSE_BATCOMPLETE[] = {2, 0xAA, 0x00};
__code uint8_t MOUSE_ID[] = {2, 0xAB, 0x83};
*/

	else if (port == PORT_MOUSE)
	{

		switch (ports[port].recvout)
		{
		// Reset
		case 0xFF:
			SendMouse(0xFA); // ACK
			SendMouse(0xAA); // POST OK
			SendMouse(0x00); // Squeek Squeek I'm a mouse
			break;

		default:
			SendMouse(0xFA); // ACK
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