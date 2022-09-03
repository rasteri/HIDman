#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
#include "uart.h"
#include "protocol.h"
#include "ps2.h"


SBIT(LED, 0x90, 6);
SBIT(KEY_CLOCK, 0xB0, 4);
SBIT(KEY_DATA, 0xB0, 5);

SBIT(MOUSE_CLOCK, 0xA0, 0);
SBIT(MOUSE_DATA, 0xA0, 1);

__xdata uint8_t repeatDiv = 0;

// timer should run at 48MHz divided by (0xFFFF - (TH0TL0))
// i.e. 60khz
void mTimer0Interrupt(void) __interrupt(1)
{
	// Reload to 60KHz
	TH0 = 0xFC;
	TL0 = 0xDF;

	PS2ProcessPort(PORT_KEY);
	PS2ProcessPort(PORT_MOUSE);

	// now handle keyboard typematic repeat timers
	// divide down to 15KHz to make maths easier
	if (++repeatDiv == 4){
		RepeatTimer();
		repeatDiv = 0;
	}
}

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
	P2_DIR = 0xff;		// output
	P2_PU = 0x00;		// pull up - change this to 0x00 when we add the 5v pullup

	// timer0 setup
	TMOD = (TMOD & 0xf0) | 0x01; // mode 1 (16bit no auto reload)
	T2MOD |= 0b10010000; // fast mode (fsys) - should be 48MHz

	// preload to 60KHz
	TH0 = 0xFC;
	TL0 = 0xDF;

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;
	EA = 1;	 // enable all interrupts

	P0_DIR = 0b01110000; // LEDs as output
	P0_PU = 0x00;
	P0 = 0x00; // all lit
	P0 = 0b01110000; // none lit
	P0 = 0b00110000; // one lit

	DEBUG_OUT("Ready\n");
	//sendProtocolMSG(MSG_TYPE_STARTUP,0, 0x00, 0x00, 0x00, 0);

	OutPort(PORT_KEY, DATA, 1);
	OutPort(PORT_KEY, CLOCK, 1);

	OutPort(PORT_MOUSE, DATA, 1);
	OutPort(PORT_MOUSE, CLOCK, 1);

	while (1)
	{
		if (!(P4_IN & (1 << 6)))
			runBootloader();
		processUart();
		s = checkRootHubConnections();
		pollHIDdevice();
		HandleRepeats();
	}
}