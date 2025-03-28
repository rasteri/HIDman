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
#define TESTVERBOSE 1

/*
Things we might want to test:
- HID report parsing. Rather than counting segs, search for segs that are the correct type, in the correct startbit, etc
- HID report processing. Can actually dig the bits out and get a sensible result at the end
- Ring buffer functions (when we write them lol)
- Presskey/releasekey actually gives a sensible result out of the ringbuffer at the end
- Test entire thing - parse a report, send some events, make sure they come out of ringbuffer at end

Probably gonna need seperate executables for different tests because of limited code space
 
*/

// mask off the number of bits
__code uint16_t bitMasks16[] = {
    0x0000, 
    0x0001, 0x0003, 0x0007, 0x000f, 0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0fFF, 0x1fFF, 0x3fFF, 0x7fFF, 0xFFFF
 };
__xdata USB_HUB_PORT UsbDev;

__xdata uint8_t KeyboardTestDataD[] = { 0, 0, 4, 0, 0, 0, 0, 0 };
__xdata uint8_t KeyboardTestDataU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };



__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
__xdata uint8_t FakeG304TestData2[] = { 0x02, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00  };
//__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x01 , 0x80 , 0x04 , 0x00 , 0x00 };
//__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x7F , 0x30 , 0x00 , 0x00 , 0x00  };
__xdata uint8_t FakeG304TestData3[] = { 0x02, 0x00, 0x02 , 0xB0 , 0xFF , 0x00 , 0x00 };
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
    printf("Button %x set to %x\n", Button, value);
}

bool SendKeyboard(__code uint8_t *chunk)
{
    if (chunk == NULL)
        return 1;
        
    printf ("Keyboard Sent %x : ", chunk[0]);
    for (uint8_t x = 1; x <= chunk[0]; x++)
    {
        printf("%02X ", chunk[x]);
    }
    printf("\n");
    return 1;
}

void MouseMove(int32_t DeltaX, int32_t DeltaY, int32_t DeltaZ){
    printf("Mouse : %ld,", DeltaX);
    printf("%ld,", DeltaY);
    printf("%ld\n", DeltaZ);
}

void Menu_Press_Key(uint8_t key)
{

}


void TestBitOffsets(uint8_t reportSize, uint16_t startBit, __xdata uint8_t *TestData, uint8_t TestDataLen){

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
}

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

    //testrand();

    /*static __xdata uint8_t FunTestData1[] = { 0x01, 0x21, 0x43, 0x65};
    TestBitOffsets(12, 8, FunTestData1, 4);

    static __xdata uint8_t FunTestData2[] = {0x01, 0x42, 0x86, 0xCA, 0x00}; //needs a 5th byte so it's blank coz it'll shift
    TestBitOffsets(12, 9, FunTestData2, 5);

    static __xdata uint8_t FunTestData3[] = {0x01, 0x84, 0x0c, 0x95, 0x01};
    TestBitOffsets(12, 10, FunTestData3, 5);

    static __xdata uint8_t FunTestData4[] = {0x01, 0x08, 0x19, 0x2A, 0x03};
    TestBitOffsets(12, 11, FunTestData4, 5);*/


    static __xdata uint8_t FunTestData5[] = {0x01, 0x10, 0x32, 0x54, 0x76};
    TestBitOffsets(16, 8, FunTestData5, 5);

    static __xdata uint8_t FunTestData6[] = {0x01, 0x20, 0x64, 0xA8, 0xEC, 0x00}; //needs a 6th byte so it's blank coz it'll shift
    TestBitOffsets(16, 9, FunTestData6, 6);

    printf("Parser tests passed\n");

    for (;;)
    {
        //printf("v");
    }
}