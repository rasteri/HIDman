/*

testsegextract.c

Tests whether SegExtractValue can pull the bits out of a report
At various start offsets and segment sizes

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
#include "testdata.h"
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"
#include "scancode.h"
#include "ps2mapping.h"
#include "processreport.h"
#define TESTVERBOSE 1

// mask off the number of bits
__code uint32_t bitMasks32[] = {
    0x0000, 
    0x0001, 0x0003, 0x0007, 0x000f, 0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0fFF, 0x1fFF, 0x3fFF, 0x7fFF, 0xFFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000fFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0fFFFFFF, 0x1fFFFFFF, 0x3fFFFFFF, 0x7fFFFFFF, 0xFFFFFFFF
 };
__xdata USB_HUB_PORT UsbDev;



// Who says you can't do dependency injection in C
__xdata bool MenuActive = 0;

__at(0xF000) __code Settings DefSettings = {
    0x54178008,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    MODE_PS2
};

void MouseSet(uint8_t Button, uint8_t value)
{
}

bool SendKeyboard(__code uint8_t *chunk)
{
    return 1;
}

void MouseMove(int32_t DeltaX, int32_t DeltaY, int32_t DeltaZ){
}

void Menu_Press_Key(uint8_t key)
{

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
    for (uint8_t c = 0; c < 100; c++) {
        ogtestdata = rand32() & bitMasks32[reportSize];
        testdata = ogtestdata << startBit;
        afterdata = SegExtractValue(&testSeg, (__xdata uint8_t *)(&testdata));
        assert(ogtestdata == afterdata);
    }
    return 1;
}

/*void TestBitOffsets(uint8_t reportSize, uint16_t startBit){

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    pInterface->Reports = ListAdd(pInterface->Reports, sizeof(HID_REPORT), 1);

    HIDParseState.hidGlobal.reportID = 1;
    HIDParseState.hidGlobal.reportSize = reportSize;
    HIDParseState.hidGlobal.reportCount = 1;
    HIDParseState.hidGlobal.usagePage = REPORT_USAGE_PAGE_GENERIC;
    HIDParseState.appUsage = REPORT_USAGE_MOUSE;
    HIDParseState.startBit = startBit;
    HIDParseState.appUsagePage = REPORT_USAGE_PAGE_GENERIC;
    HIDParseState.arrUsage[0] = USAGE_X;
    HIDParseState.arrUsage[1] = USAGE_Y;
    HIDParseState.usagePtr = 2;
    pInterface->usesReports = true;
    CreateUsageMapping(pInterface);
    DumpHID(pInterface);

    ParseReport(pInterface, TestDataLen, TestData);
}*/

void testrand(){
    uint32_t heh = rand32();
    for (int x = 0; x < 100; x++){
        heh = rand32();
        printf("%lX\n", heh);
    }
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


    for (int size=1; size <= 32; size++) {
        for (int sb=0; sb <= 32 - size; sb++){
            printf ("%d-%d ", size, sb);
            assert(TestExtractValue(size, sb));
        }
    }


    printf("Parser tests passed\n");

    for (;;)
    {
        //printf("v");
    }
}