/*

    End-to-end tests for HIDman
    Tests entire thing - parses a report, sends some events, makes sure they come out of ringbuffer at end

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "type.h"
#include "ch559.h"
#include "system.h"
#include "defs.h"
#include "usbdef.h"
#include "usbhost.h"
#include "menu.h"
#include "data.h"
#include "settings.h"
#include "andyalloc.h"
#include "keyboardled.h"
#include "parsedescriptor.h"
#include "ps2protocol.h"
#include "ps2.h"
#include "testdata.h"
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"
#include "scancode.h"
#include "processreport.h"
#define TESTVERBOSE 1

__xdata USB_HUB_PORT UsbDev;


void main()
{
    // timer0 setup
	TMOD = (TMOD & 0xf0) | 0x02; // mode 1 (8bit auto reload)
	TH0 = 0x00;					 // I dunno

	TR0 = 1; // start timer0
	ET0 = 1; //enable timer0 interrupt;

	EA = 1;	 // enable all interrupts

    UART_Init();

    printstackpointer();
    /*uint32_t value = 0x00000FFF;
    value = 0x00000F00;
    printf("sign %lX\n", (uint32_t)SIGNEX(value, 11));
    while(1);*/

    //TestG304();
    TestStandardKeyboard();
    TestStandardMouse();
    //TestSegExtracts();  
    TestFakeG304();
    TestCheapoGamepad();
    TestPSXAdapter();

    printf("ALL TESTS PASSED\n");

    for (;;)
    {
        //printf("v");
    }
}