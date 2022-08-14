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
SBIT(KEY_DATA, 0xA0, 1); // port 2.1

SBIT(MOUSE_CLOCK, 0xA0, 2); // port 2.2
SBIT(MOUSE_DATA, 0xA0, 3); // port 2.3


const uint8_t KEYA_MAKE[] = {1, 0x1C};
const uint8_t KEYA_BREAK[] = {2, 0xF0, 0x1C};
const uint8_t KEYB_MAKE[] = {1, 0x32};
const uint8_t KEYB_BREAK[] = {2, 0xF0, 0x32};

void OutPort (unsigned char port, unsigned char channel, bool val){
	if (port == PORT_KEY)
		if (channel == CLOCK)
			KEY_CLOCK = val;
		else
			KEY_DATA = val;
		
	else
		if (channel == CLOCK)
			MOUSE_CLOCK = val;
		else
			MOUSE_DATA = val;
}

bool GetPort (unsigned char port, unsigned char channel){
	if (port == PORT_KEY)
		if (channel == CLOCK)
			return KEY_CLOCK;
		else
			return KEY_DATA;
		
	else
		if (channel == CLOCK)
			return MOUSE_CLOCK;
		else
			return MOUSE_DATA;
}

void SendPS2(ps2port *port, const uint8_t *chunk){
	// check for full
	if ((port->sendBuffEnd + 1) % 64 == port->sendBuffStart){
		// do nothing
	} 
	else {
		port->sendBuff[port->sendBuffEnd] = chunk;
		port->sendBuffEnd++;
	}
}

void ps2stuff(ps2port *port){
	uint8_t *chunk;
	bool reEnter = 0;
	do {
		reEnter = 0;
		switch (port->state){
			case S_INIT:
				port->state = S_IDLE;
				reEnter = 1;
			break;
			
			case S_IDLE:
				// check to see if host is trying to inhibit
				/*if (!GetPort(port->port, CLOCK)){
					// host is trying to inhibit, make sure data is high so we can detect it if it goes low
					OutPort(port->port, DATA, 1);
					port->state = S_INHIBIT;
				}
				else {
					
				}*/

				//if buffer not empty
				if (port->sendBuffEnd != port->sendBuffStart){
					port->state = S_SEND_CLOCK_LOW;
					reEnter = 1;
				}
				
				
			break;
			
			case S_SEND_CLOCK_LOW:
				// check to see if host is trying to inhibit
				/*if (!GetPort(port->port, CLOCK)){
					// host is trying to inhibit, make sure data is high so we can detect it if it goes low
					OutPort(port->port, DATA, 1);
					port->state = S_INHIBIT;
				}
				else {*/
					// start bit
					if (port->bitnum == 0){
						OutPort(port->port, DATA, 0);
					}
					
					// data bits
					else if (port->bitnum > 0 && port->bitnum < 9){
						// set current bit data
						OutPort(port->port, DATA, port->data & 0x01);
						
						// calc parity and shift in preperation for next bit
						port->parity = port->parity ^ (port->data & 0x01);
						port->data = port->data >> 1;
					}
					
					// parity
					else if (port->bitnum == 9){
						OutPort(port->port, DATA, port->parity & 0x01);
					}
					
					// stop bit
					else if (port->bitnum == 10){
						OutPort(port->port, DATA, 1);
					}
					
					//make clock low
					OutPort(port->port, CLOCK, 0);
					
					port->bitnum++;
					
					port->state = S_SEND_CLOCK_HIGH;
				//}
				
			break;
			
			case S_SEND_CLOCK_HIGH:
				
				//make clock high
				OutPort(port->port, CLOCK, 1);
				
				// if final bit, move onto next byte
				if (port->bitnum == 11){
					port->parity = 0;
					port->bitnum = 0;

					// get current chunk
					chunk = port->sendBuff[port->sendBuffStart];

					port->bytenum++;

					// if we've run out of bytes in this chunk
					if (port->bytenum == chunk[0]){
						// move onto next chunk
						port->sendBuffStart = (port->sendBuffStart + 1) % 64;
						port->bytenum = 0;
						chunk = port->sendBuff[port->sendBuffStart];

						// if we've run out of chunks, i.e. buffer empty
						if (port->sendBuffStart == port->sendBuffEnd){
							port->state = S_IDLE;
							break;
						}

					}

					port->data = chunk[port->bytenum + 1];
					port->state = S_SEND_CLOCK_LOW;
				}

			
			break;
			
			case S_INHIBIT:
			
				// wait for host to release clock
				if (GetPort(port->port, CLOCK)){
					// if data line low then host wants to transmit
					if (!GetPort(port->port, DATA)){
						port->state = S_RECIEVE_CLOCK_LOW;
					}
				}
			
			break;
		}
	} while (reEnter);
}