/*
	ps2protocol.c
	
	Handles the higher-level parts of the PS/2 protocol
	Responding to host commands
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
#include "menu.h"
#include "mouse.h"
#include "pwm.h"
#include "keyboardled.h"
#include "xt.h"
#include "settings.h"
#include "system.h"
#include "scancode.h"
#include "processreport.h"

__code unsigned char KEY_ACK[] = {1, 0xFA};
__code unsigned char KEY_BATCOMPLETE[] = {1, 0xAA};
__code unsigned char KEY_ID[] = {2, 0xAB, 0x83};

__code unsigned char KEY_SCANCODE_2[] = {1, 0x02};
__code unsigned char KEY_ECHO[] = {1, 0xEE};
__code unsigned char KEY_ERROR[] = {1, 0xFE};


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
				// Start a 500ms countdown then send BAT OK (happens in main.c)
				BatCounter = 500;
				ports[PORT_KEY].recvstate = R_IDLE;
				break;

			// not a command byte - this is fine if we're in another sstate, otherwise we send an error
			default:
				switch (ports[PORT_KEY].recvstate)
				{

					case R_LEDS:
						SimonSaysSendKeyboard(KEY_ACK);
						SetKeyboardLedStatusFromPS2(ports[PORT_KEY].recvout);
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
		
			// push command to buffer
			memmove(MouseBuffer+1, MouseBuffer, MOUSE_BUFFER_SIZE-1);
			MouseBuffer[0] = ports[port].recvout;

			switch (ports[port].recvout)
			{
				
			// Set Scaling 1:1
			case 0xE6:
				SimonSaysSendMouse1(0xFA); // ACK
				Ps2MouseSetScaling(MOUSE_PS2_SCALING_1X);
				break;
				
			// Set Scaling 2:1
			case 0xE7: 
				SimonSaysSendMouse1(0xFA); // ACK
				Ps2MouseSetScaling(MOUSE_PS2_SCALING_2X);
				break;
				
			// Set Resolution (need 1 additional data byte)
			case 0xE8: 
				SimonSaysSendMouse1(0xFA); // ACK
				break;
			
			// Status Request
			case 0xE9:							
				// TODO: construct bytes from real state
				SimonSaysSendMouse1(0xFA);		// ACK
				SimonSaysSendMouse3(0b00100000, // Stream Mode, Scaling 1:1, Enabled, No buttons pressed
									0x02,		// Resolution 4 counts/mm
									100);		// Sample rate 100/sec
				break;

			// ID
			case 0xF2:
				SimonSaysSendMouse1(0xFA); // ACK
				SimonSaysSendMouse1((&OutputMice[MOUSE_PORT_PS2])->Ps2Type); // Mouse type
				break;

			// Enable Reporting
			case 0xF4: 
				SimonSaysSendMouse1(0xFA); // ACK
				Ps2MouseSetReporting(MOUSE_PS2_REPORTING_ON);
				break; 
				
			// Disable Reporting
			case 0xF5: 
				SimonSaysSendMouse1(0xFA); // ACK
				Ps2MouseSetReporting(MOUSE_PS2_REPORTING_OFF);
				break;
			
			// Set Defaults
			case 0xF6: 
				SimonSaysSendMouse1(0xFA); // ACK
				Ps2MouseSetDefaults();
				break;
			
			// Reset
			case 0xFF:
				SimonSaysSendMouse1(0xFA); // ACK
				SimonSaysSendMouse1(0xAA); // POST OK
				SimonSaysSendMouse1(0x00); // Squeek Squeek I'm a mouse
				Ps2MouseSetType(MOUSE_PS2_TYPE_STANDARD);
				Ps2MouseSetDefaults();
				break;

			// unimplemented command
			case 0xEA: // Set Stream Mode
			case 0xEB: // Read Data
			case 0xEC: // Reset Wrap Mode
			case 0xEE: // Set Wrap Mode
			case 0xF0: // Remote Mode
			case 0xF3: // Set Sample Rate
			case 0xFE: // Resend
			default:   // argument from command?
				if (MouseBuffer[1] == 0xE8) 
				{
					// previous command was set Resolution, this should be actual resolution
					SimonSaysSendMouse1(0xFA); // ACK
					Ps2MouseSetResolution(MouseBuffer[0]);
				} 
				else
				{
					SimonSaysSendMouse1(0xFA); // Just smile and nod
				}
				break;
			}
		}
		
		if (HMSettings.Intellimouse){
			// enable intellimouse support if driver tried to to detect it
			if (memcmp(MouseBuffer, "\x50\xF3\x64\xF3\xC8\xF3", 6) == 0)
			{
				Ps2MouseSetType(MOUSE_PS2_TYPE_INTELLIMOUSE_3_BUTTON);
			}
			else if (memcmp(MouseBuffer, "\x50\xF3\xC8\xF3\xC8\xF3", 6) == 0)
			{
				Ps2MouseSetType(MOUSE_PS2_TYPE_INTELLIMOUSE_5_BUTTON);
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