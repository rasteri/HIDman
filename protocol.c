/*
	protocol.c
	
	Handles the higher-level parts of the PS/2 protocol
	HID conversion, responding to host commands

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

// repeatState -
// if positive, we're delaying - count up to repeatDelay then go negative
// if negative, we're repeating - count down to repeatRate then return to -1
// if zero, we ain't repeating
__xdata volatile int16_t RepeatState = 0;

__xdata volatile uint8_t RepeatKey = 0x00;
__xdata int16_t RepeatDelay = 7500;
__xdata int16_t RepeatRate = -1000;

__xdata char lastKeyboardHID[8];

// runs in interupt to keep timings
void RepeatTimer()
{
	if (RepeatState > 0)
		RepeatState++;
	else if (RepeatState < 0)
		RepeatState--;
}
__code int16_t DelayConv[] = {
	3750,
	7500,
	11250,
	15000};

__code int16_t RateConv[] = {
	-500,
	-562,
	-625,
	-688,
	-725,
	-811,
	-877,
	-938,
	-1000,
	-1128,
	-1250,
	-1376,
	-1500,
	-1630,
	-1744,
	-1875,
	-2000,
	-2239,
	-2500,
	-2727,
	-3000,
	-3261,
	-3488,
	-3750,
	-4054,
	-4545,
	-5000,
	-5556,
	-6000,
	-6522,
	-7143,
	-7500};

// Runs in main loop
void HandleRepeats()
{
	if (RepeatState > RepeatDelay)
	{
		SetRepeatState(-1);
	}
	else if (RepeatState < RepeatRate)
	{
		SendKeyboard(HIDtoPS2_Make[RepeatKey]);
		SetRepeatState(-1);
	}
}

void SendHIDPS2(unsigned short length, __xdata unsigned char devnum, unsigned char type, unsigned char __xdata *msgbuffer)
{
	bool brk = 0, make = 0;
	uint8_t currcode;
	signed char x, y;
	switch (type)
	{
	case REPORT_USAGE_KEYBOARD:

		// do special keys first
		if (msgbuffer[0] != lastKeyboardHID[0])
		{
			uint8_t rbits = msgbuffer[0];
			uint8_t pbits = lastKeyboardHID[0];

			// iterate through bits and compare to previous to see whats changed
			for (uint8_t j = 0; j < 8; j++)
			{
				// this bit has changed
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
		}

		// iterate through all the HID bytes to see what's changed since last time
		for (uint8_t i = 2; i < 8; i++)
		{
			// key was pressed last time
			if (lastKeyboardHID[i])
			{

				// assume this will be a break code
				brk = 1;

				// see if this code is still present in current poll
				for (uint8_t j = 2; j < 8; j++)
				{
					if (lastKeyboardHID[i] == msgbuffer[j])
					{
						// if so, do not break
						brk = 0;
						break;
					}
				}

				if (brk)
				{
					// if the key we just released is the one that's repeating then stop
					if (lastKeyboardHID[i] == RepeatKey)
					{
						RepeatKey = 0;
						SetRepeatState(0);
					}

					//DEBUG_OUT("Break %x\n", lastHID[devnum][i]);
					// no break code for pause key, for some reason
					if (lastKeyboardHID[i] == 0x48)
						continue;

					//send the break code
					if (lastKeyboardHID[i] <= 0x67)
						SendKeyboard(HIDtoPS2_Break[lastKeyboardHID[i]]);
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
					if (msgbuffer[i] == lastKeyboardHID[j])
					{
						// if so, no need to make
						make = false;
						break;
					}
				}

				if (make)
				{

					if (msgbuffer[i] <= 0x67)
					{
						SendKeyboard(HIDtoPS2_Make[msgbuffer[i]]);
						RepeatKey = msgbuffer[i];
						SetRepeatState(1);
					}
				}
			}
		}

		for (int i = 0; i < length; i++)
		{
			lastKeyboardHID[i] = msgbuffer[i];
		}

		break;

	case REPORT_USAGE_MOUSE:

		/*ANDYS_DEBUG_OUT("Mouse : ");
		for (int p = 0; p < length; p++)
			ANDYS_DEBUG_OUT("%x ", msgbuffer[p]);
		ANDYS_DEBUG_OUT("\n");*/

		//HID :
		//byte 0 is buttons
		//byte 1 is x movement (8 bit signed)
		//byte 2 is y movement (8 bit signed)

		x = (signed char)msgbuffer[1];
		y = (signed char)msgbuffer[2];

		// PS2 mice have the y-axis inverted from HID
		y = -y;

		// First PS2 byte
		// bit3 always set
		uint8_t tmp = 0x08;

		// bottom 3 bits of button format is the same
		tmp |= msgbuffer[0] & 0x07;

		// X sign
		tmp |= ((x & 0x80) >> 3);

		// Y sign
		tmp |= ((y & 0x80) >> 2);

		SendMouse3(tmp, (uint8_t)x, (uint8_t)y);

		break;

		/*	case 0x04: //joystick

		if (msgbuffer[0] == 1)
		{
			// iterate through all the HID bytes to see what's changed since last time
			for (uint8_t i = 1; i < 8; i++)
			{
				if (lastHID[devnum][i] != msgbuffer[i])
				{
					switch (i)
					{
					case 1: // right analog stick Y
						break;
					case 2: // right analog stick X
						break;
					case 3: // left analog stick X
						// breaks
						if (msgbuffer[3] >= 64 && lastHID[devnum][3] < 64) // not left but left last time
							SendKeyboard(KEY_LEFT_BREAK);
						if (msgbuffer[3] <= 192 && lastHID[devnum][3] > 192) // not right but right last time
							SendKeyboard(KEY_RIGHT_BREAK);
						// makes
						if (msgbuffer[3] < 64 && lastHID[devnum][3] >= 64) // left and not left last time
							SendKeyboard(KEY_LEFT_MAKE);
						if (msgbuffer[3] > 192 && lastHID[devnum][3] <= 192) // right and not right last time
							SendKeyboard(KEY_RIGHT_MAKE);

						break;
					case 4: // left analog stick Y
						// breaks
						if (msgbuffer[4] >= 64 && lastHID[devnum][4] < 64) // not up but up last time
							SendKeyboard(KEY_UP_BREAK);
						if (msgbuffer[4] <= 192 && lastHID[devnum][4] > 192) // not down but down last time
							SendKeyboard(KEY_DOWN_BREAK);
						// makes
						if (msgbuffer[4] < 64 && lastHID[devnum][4] >= 64) // up and not up last time
							SendKeyboard(KEY_UP_MAKE);
						if (msgbuffer[4] > 192 && lastHID[devnum][4] <= 192) // down and not down last time
							SendKeyboard(KEY_DOWN_MAKE);
						break;
					case 5:
						// breaks
						if (msgbuffer[5] & 0x10 && !(lastHID[devnum][5] & 0x10))
							SendKeyboard(KEY_LCTRL_MAKE);
						if (msgbuffer[5] & 0x20 && !(lastHID[devnum][5] & 0x20))
							SendKeyboard(KEY_SPACE_MAKE);
						if (msgbuffer[5] & 0x40 && !(lastHID[devnum][5] & 0x40))
							SendKeyboard(KEY_LALT_MAKE);
						if (msgbuffer[5] & 0x80 && !(lastHID[devnum][5] & 0x80))
							SendKeyboard(KEY_ENTER_MAKE);
						// makes
						if (!(msgbuffer[5] & 0x10) && lastHID[devnum][5] & 0x10)
							SendKeyboard(KEY_LCTRL_BREAK);
						if (!(msgbuffer[5] & 0x20) && lastHID[devnum][5] & 0x20)
							SendKeyboard(KEY_SPACE_BREAK);
						if (!(msgbuffer[5] & 0x40) && lastHID[devnum][5] & 0x40)
							SendKeyboard(KEY_LALT_BREAK);
						if (!(msgbuffer[5] & 0x80) && lastHID[devnum][5] & 0x80)
							SendKeyboard(KEY_ENTER_BREAK);
						break;
					case 6:
						break;
					}
				}

				lastHID[devnum][i] = msgbuffer[i];
			}
		}
		break;*/

	default:
		// byte 0 is the subdevice number
		// byte 1/2 are right analog stick Y/X
		// byte 3/4 are left analog stick/DPAD X/Y
		// byte 5 -
		//   bits 0-3 are always 1
		//   bits 4-7 are thumb buttons
		// byte 6 -
		//   bits 0-3 are shoulder buttons
		//   bits 4-7 are always 0

		/*if (msgbuffer[0] == 1)
		{

			
		}*/
		break;
	}

}

void TypematicDefaults()
{
	RepeatDelay = DelayConv[0x01]; // 500ms
	RepeatRate = RateConv[0x0B];   // 10.9cps
}

void HandleReceived(uint8_t port)
{
	if (port == PORT_KEY)
	{
		// see if this is a command byte - these should override any state we're already in so check these first
		switch (ports[PORT_KEY].recvout)
		{

		// set LEDs
		case 0xED:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_LEDS;
			break;

		// Echo
		case 0xEE:
			SimonSaysSendKeyboard(KEY_ECHO);
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// Set/Get scancode set
		case 0xF0:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_SCANCODESET;
			break;

		// ID
		case 0xF2:
			SimonSaysSendKeyboard(KEY_ACK);
			SimonSaysSendKeyboard(KEY_ID);
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// set repeat
		case 0xF3:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_REPEAT;
			break;

		// Enable (TODO : actually do this)
		case 0xF4:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// Disable (TODO : actually do this)
		case 0xF5:
			SimonSaysSendKeyboard(KEY_ACK);
			TypematicDefaults();
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// Set Default
		case 0xF6:
			SimonSaysSendKeyboard(KEY_ACK);
			TypematicDefaults();
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// Set All Keys whatever (TODO : Actually do this)
		case 0xF7:
		case 0xF8:
		case 0xF9:
		case 0xFA:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// Set specific Keys whatever (TODO : Actually do this)
		case 0xFB:
		case 0xFC:
		case 0xFD:
			SimonSaysSendKeyboard(KEY_ACK);
			ports[PORT_KEY].recvstate = R_KEYLIST;
			break;

		// resend - TODO figure out how to deal with this
		case 0xFE:
			SimonSaysSendKeyboard(KEY_ACK);
			break;

		// reset
		case 0xFF:
			SimonSaysSendKeyboard(KEY_ACK);
			TypematicDefaults();
			SimonSaysSendKeyboard(KEY_BATCOMPLETE);
			ports[PORT_KEY].recvstate = R_IDLE;
			break;

		// not a command byte - this is fine if we're in another state, otherwise we send an error
		default:
			switch (ports[PORT_KEY].recvstate)
			{

			case R_LEDS:
				// TODO blinkenlights
				SimonSaysSendKeyboard(KEY_ACK);
				ports[PORT_KEY].recvstate = R_IDLE;
				break;

			case R_REPEAT:
				SimonSaysSendKeyboard(KEY_ACK);
				// Bottom 5 bits are repeate rate
				RepeatRate = RateConv[ports[PORT_KEY].recvout & 0x1F];

				// top 3 bits are delay time
				RepeatDelay = DelayConv[(ports[PORT_KEY].recvout >> 5) & 0x03];

				ports[PORT_KEY].recvstate = R_IDLE;
				break;

			case R_SCANCODESET:
				SimonSaysSendKeyboard(KEY_ACK);

				// if request is zero then PC is expecting us to send current scan code set number#
				// TODO : allow changing scancode set and reporting correctly
				if (ports[PORT_KEY].recvout == 0)
					SimonSaysSendKeyboard(KEY_SCANCODE_2);

				ports[PORT_KEY].recvstate = R_IDLE;
				break;

			case R_KEYLIST:
				// Keep sending ACKs until we get a valid command scancode (which will be dealt with by the previous section)
				// TODO : actually log these keys and apply their settings appropriately
				// (not that I've ever seen any programs actually require this)
				SimonSaysSendKeyboard(KEY_ACK);
				break;

			case R_IDLE:
			default:
				// this means we receieved a non-command byte when we are expecting one, send error
				SimonSaysSendKeyboard(KEY_ERROR);
				ports[PORT_KEY].recvstate = R_IDLE;
				break;
			}
		}

		// If we're not expecting more stuff,
		// unlock the send buffer so main loop can send stuff again
		if (ports[PORT_KEY].recvstate == R_IDLE)
		{
			ports[PORT_KEY].sendDisabled = 0;
		}
	}

	else if (port == PORT_MOUSE)
	{
		switch (ports[PORT_KEY].recvstate)
		{
		case R_IDLE:

			switch (ports[port].recvout)
			{
			case 0xE9:							// Status Request
				SimonSaysSendMouse1(0xFA);		// ACK
				SimonSaysSendMouse3(0b00100000, // Stream Mode, Scaling 1:1, Enabled, No buttons pressed
									0x02,		// Resolution 4 counts/mm
									100);		// Sample rate 100/sec
				break;

			// ID
			case 0xF2:
				SimonSaysSendMouse1(0xFA); // ACK
				SimonSaysSendMouse1(0x00); // Standard mouse
				break;

			// Reset
			case 0xFF:
				SimonSaysSendMouse1(0xFA); // ACK
				SimonSaysSendMouse1(0xAA); // POST OK
				SimonSaysSendMouse1(0x00); // Squeek Squeek I'm a mouse
				break;

			// unimplemented command
			case 0xE6: // Set Scaling 1:1
			case 0xE7: // Set Scaling 2:1
			case 0xE8: // Set Resolution
			case 0xEA: // Set Stream Mode
			case 0xEB: // Read Data
			case 0xEC: // Reset Wrap Mode
			case 0xEE: // Set Wrap Mode
			case 0xF0: // Remote Mode
			case 0xF3: // Set Sample Rate
			case 0xF4: // Enable Reporting
			case 0xF5: // Disable Reporting
			case 0xF6: // Set Defaults
			case 0xFE: // Resend
			default:   // argument from command?

				SimonSaysSendMouse1(0xFA); // Just smile and nod
				break;
			}
		}
		// If we're not expecting more stuff,
		// unlock the send buffer so main loop can send stuff again
		if (ports[PORT_MOUSE].recvstate == R_IDLE)
		{
			ports[PORT_MOUSE].sendDisabled = 0;
		}
	}
}