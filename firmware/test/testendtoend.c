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


__xdata uint8_t MouseTestData[] = { 0, 0, 0 };









void TestStandardMouse(void){

    InitPS2Ports();
    InitMice();

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);
    assert(!InterfaceParseReportDescriptor(pInterface, StandardMouseDescriptor, 50));

    // put a bunch of stuff in the buffer (should be 60 entries)
    int c;
    uint8_t * chonk;

    // test buttons
    // no movement
    MouseTestData[1] = 0;
    MouseTestData[2] = 0;

    // 3 buttons gives us 8 combinations
    for (c = 1; c < 8; c++){
        // press
        MouseTestData[0] = c;
        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        assert (chonk != NULL);
        assert ((chonk[1] & 0x07) == c);

        // unpress
        MouseTestData[0] = 0;
        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        assert (chonk != NULL);
        assert ((chonk[1] & 0x07) == 0);
    }

    // random movement test
    for (c = 0; c < 255; c++){
        MouseTestData[0];
        MouseTestData[1] = (uint8_t)rand();
        MouseTestData[2] = (uint8_t)rand();

        // zero sized moves might not trigger a chonk
        if (MouseTestData[1] == 0) continue;
        if (MouseTestData[2] == 0) continue;

        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        //printf("%X %X - %X %X %X %X\n", MouseTestData[1], MouseTestData[2], chonk[0], chonk[1], chonk[2], (uint8_t)(255-chonk[3])+(uint8_t)1);
        assert (chonk != NULL);
        assert (MouseTestData[1] == chonk[2]);
        assert (MouseTestData[2] == (uint8_t)(( 255 - chonk[3]) + 1));
    }



    printf("End-to-end Standard Mouse Test Passed\n");

    //DumpHID(pInterface);
}

bool TestExtractValue(uint8_t reportSize, uint16_t startBit){

    static __xdata HID_SEG testSeg;
    
    testSeg.startBit = startBit;
    testSeg.reportSize = reportSize;

    //DumpHID(pInterface);

    static __xdata uint32_t ogtestdata;
    static __xdata uint32_t testdata;
    static __xdata uint32_t afterdata;

    //start with all zeros
    testdata = 0;
    afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
    assert(testdata == afterdata);

    //now all 1s
    ogtestdata = (0xFFFFFFFF & bitMasks32[reportSize]);
    testdata = ogtestdata << startBit;
    afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
    assert(ogtestdata == afterdata);

    //now a few random values
    for (uint8_t c = 0; c < 20; c++) {
        ogtestdata = rand32() & bitMasks32[reportSize];
        testdata = ogtestdata << startBit;
        afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
        assert(ogtestdata == afterdata);
    }

    // if we're testing a single bit, set JUST that bit and make sure none of the other bits return 1
    if (reportSize == 1) {
        testdata = (uint32_t)0x01 << (uint32_t)startBit;

        for (uint8_t d = 0; d < 32; d++){
            testSeg.startBit = d;

            // the bit we're looking for should be 1
            if (d == startBit) {

                assert (SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata)) == 0x01);
            }

            // the bits we're not looking for should be zero
            else {
                assert (SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata)) != 0x01);
            }
        }
    }

    return 1;
}

void TestSegExtracts(){
    for (int size=1; size <= 32; size++) {
        for (int sb=0; sb <= 32 - size; sb++){
            //printf ("%d-%d ", size, sb);
            assert(TestExtractValue(size, sb));
        }
    }

    printf("HID Segment Offset/Size Test Passed\n");
}







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