#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "ps2.h"

SBIT(LED, 0x90, 6);

void mTimer0Interrupt( void) __interrupt (1)
{	
	TH0 = 0xff;
	TL0 = 0xB8;
	PS2ProcessPort(PORT_KEY);
	//PS2ProcessPort(&mouse);
}

SBIT(KEY_CLOCK, 0xA0, 0); // port 2.0
SBIT(KEY_DATA, 0xA0, 1);  // port 2.1

SBIT(MOUSE_CLOCK, 0xA0, 2); // port 2.2
SBIT(MOUSE_DATA, 0xA0, 3);	// port 2.3

void main()
{
    unsigned char s;
	uint8_t keyindex;
    initClock();
    initUART0(1000000, 1);
    DEBUG_OUT("Startup\n");
    resetHubDevices(0);
    resetHubDevices(1);
    initUSB_Host();
	
	//port2 setup
	PORT_CFG |= bP2_OC; // open collector
	P2_DIR = 0xff; // output
	P2_PU = 0xff; // pull up - change this to 0x00 when we add the 5v pullup
	
	//timer0 setup
	TMOD = (TMOD & 0xf0) | 0x01; // mode 1 (16bit no auto reload)
	//T2MOD = T2MOD & 0b01101111; // clear bTMR_CLK and bT0_CLK;
	//TH0 = 0x80; // reload to 128
	TR0 = 1;// start timer0
	ET0 = 1; //enable timer0 interrupt;
	EA = 1; // enable all interrupts
	
	
    DEBUG_OUT("Ready\n");
	//sendProtocolMSG(MSG_TYPE_STARTUP,0, 0x00, 0x00, 0x00, 0);
	
	OutPort(PORT_KEY, DATA, 1);
	OutPort(PORT_KEY, CLOCK, 1);
	
    while(1)
    {
        if(!(P4_IN & (1 << 6)))
            runBootloader();
        processUart();
        s = checkRootHubConnections();
        pollHIDdevice();

		if (ports[PORT_MOUSE].recvvalid){
			DEBUG_OUT("Received %x\n", mouse.recvout);
			ports[PORT_MOUSE].recvvalid = 0;
		}

		// if buffer isn't full, send the next code
		/*if ((keyboard.sendBuffEnd + 1) % 64 != keyboard.sendBuffStart){

			SendPS2(&keyboard, KEYA_MAKE);
			SendPS2(&keyboard, KEYB_MAKE);
			SendPS2(&keyboard, KEYC_MAKE);
			delay(50);

			SendPS2(&keyboard, KEYA_BREAK);
			SendPS2(&keyboard, KEYB_BREAK);
			SendPS2(&keyboard, KEYC_BREAK);
			delay(50);
		} */
    }
}