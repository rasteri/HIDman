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
__xdata volatile int16_t repeatState = 0;

__xdata volatile uint8_t repeatKey = 0x00;
__xdata int16_t repeatDelay = 7500;
__xdata int16_t repeatRate = -1000;

__xdata char lastKeyboardHID[8];

// runs in interupt to keep timings
void RepeatTimer()
{
	if (repeatState > 0)
		repeatState++;
	else if (repeatState < 0)
		repeatState--;
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
	if (repeatState > repeatDelay)
	{
		SetRepeatState(-1);
	}
	else if (repeatState < repeatRate)
	{
		SendKeyboard(HIDtoPS2_Make[repeatKey]);
		SetRepeatState(-1);
	}
}

void SendHIDPS2(unsigned short length, __xdata unsigned char devnum, unsigned char type, unsigned char __xdata *msgbuffer)
{
	bool brk = 0, make = 0;
	uint8_t currcode;
	signed char x,y;
	switch (type)
	{
	case REPORT_USAGE_KEYBOARD:
		/*ANDYS_DEBUG_OUT("dunno %x %x : ", type, length);
		for (int p = 0; p < length; p++)
			ANDYS_DEBUG_OUT("%x ", msgbuffer[p]);
		ANDYS_DEBUG_OUT("\n");*/
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
					if (lastKeyboardHID[i] == repeatKey)
					{
						repeatKey = 0;
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
						repeatKey = msgbuffer[i];
						SetRepeatState(1);
					}
				}
			}
		}

		for (int i=0; i < length; i++){
			lastKeyboardHID[i] = msgbuffer[i];
		}

		break;

	case REPORT_USAGE_MOUSE:

		/*DEBUG_OUT("Mouse : ");
		for (int p = 0; p < length; p++)
			DEBUG_OUT("%x ", msgbuffer[p]);
		DEBUG_OUT("\n");*/

		//HID :
		//byte 0 appears to always be 1
		//byte 1 is buttons
		//byte 2 is x movement (8 bit signed)
		//byte 3 is y movement (8 bit signed)

		x = (signed char)msgbuffer[2];
		y = (signed char)msgbuffer[3];

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

void HandleReceived(uint8_t port)
{
	if (port == PORT_KEY)
	{
		switch (ports[PORT_KEY].recvstate)
		{
		case R_IDLE:

			switch (ports[PORT_KEY].recvout)
			{
			case 0xFF:
				SimonSaysSendKeyboard(KEY_ACK);
				SimonSaysSendKeyboard(KEY_BATCOMPLETE);
				break;

			// set LEDs
			case 0xED:
				SimonSaysSendKeyboard(KEY_ACK);
				ports[PORT_KEY].recvstate = R_LEDS;
				break;

			// set repeat
			case 0xF3:
				SimonSaysSendKeyboard(KEY_ACK);
				ports[PORT_KEY].recvstate = R_REPEAT;
				break;

			// ID
			case 0xF2:
				SimonSaysSendKeyboard(KEY_ACK);
				SimonSaysSendKeyboard(KEY_ID);
				break;

			// Enable
			case 0xF4:
				SimonSaysSendKeyboard(KEY_ACK);
				break;

			// Disable
			case 0xF5:
				SimonSaysSendKeyboard(KEY_ACK);
				break;
			}

			break;

		case R_LEDS:
			// TODO blinkenlights
			ports[PORT_KEY].recvstate = R_IDLE;
			SimonSaysSendKeyboard(KEY_ACK);
			break;

		case R_REPEAT:
			// TODO repeat
			ports[PORT_KEY].recvstate = R_IDLE;
			SimonSaysSendKeyboard(KEY_ACK);
			break;
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
			// Reset
			case 0xFF:
				SimonSaysSendMouse(0xFA); // ACK
				SimonSaysSendMouse(0xAA); // POST OK
				SimonSaysSendMouse(0x00); // Squeek Squeek I'm a mouse
				break;

			// ID
			case 0xF2:
				SimonSaysSendMouse(0xFA); // ACK
				SimonSaysSendMouse(0x00); // Standard mouse
				break;

			default:
				SimonSaysSendMouse(0xFA); // ACK
				break;
			}
			ports[PORT_MOUSE].sendDisabled = 0;
			break;

			//TODO : two byte commands
		}
		// If we're not expecting more stuff,
		// unlock the send buffer so main loop can send stuff again
		if (ports[PORT_MOUSE].recvstate == R_IDLE)
		{
			ports[PORT_MOUSE].sendDisabled = 0;
		}
	}
}