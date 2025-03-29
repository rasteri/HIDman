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
__xdata USB_HUB_PORT UsbDev;

__xdata uint8_t KeyboardTestDataD[] = { 0, 0, 4, 0, 0, 0, 0, 0 };
__xdata uint8_t KeyboardTestDataU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0x21, 0x43, 0x65, 0x00 };

__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
__xdata uint8_t FakeG304TestData2[] = { 0x02, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00  };
__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x01 , 0x80 , 0x04 , 0x00 , 0x00 };
//__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x7F , 0x30 , 0x00 , 0x00 , 0x00  };
//__xdata uint8_t FakeG304TestData3[] = { 0x02, 0x00, 0x02 , 0xB0 , 0xFF , 0x00 , 0x00 };
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

void TestStandardKeyboard(void){

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);
    assert(!InterfaceParseReportDescriptor(pInterface, StandardKeyboardDescriptor, 63));

    assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataD));
    assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataU));
    assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataD));
    assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataU));
    //DumpHID(pInterface);
}

void TestG304(void){

    InitTest(&UsbDev, G304DeviceDescriptor, 18, G304ConfigDescriptor, 34);
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);
    
    assert(!InterfaceParseReportDescriptor(pInterface, G304ReportDescriptor, 105));

    assert (ParseReport(pInterface, 7 * 8, FakeG304TestData1));
    assert (ParseReport(pInterface, 7 * 8, FakeG304TestData2));
    assert (ParseReport(pInterface, 7 * 8, FakeG304TestData3));
    //DumpHID(pInterface);
}

void TestG30411(){

    InitTest(&UsbDev, G304DeviceDescriptor, 18, G304ConfigDescriptor, 34);

    assert(UsbDev.InterfaceNum == 1);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, G304ReportDescriptor, 105));

    assert(pInterface->usesReports);

    // uses report 2 for mouse
    __xdata HID_REPORT *report = (__xdata HID_REPORT *)ListGetData(pInterface->Reports, 2);

    assert(report->length == 56);
    assert(report->appUsagePage == REPORT_USAGE_PAGE_GENERIC);
    assert(report->appUsage == REPORT_USAGE_MOUSE);

    // buttons
    __xdata HID_SEG * testseg = FindSegByStartBit(report, 8);
    assert(testseg != NULL);
    assert(testseg->InputType == MAP_TYPE_BITFIELD);
    assert(testseg->OutputControl == MAP_MOUSE_BUTTON1);
    assert(testseg->OutputChannel == MAP_MOUSE);

    // X, Y, wheel
    testseg = FindSegByStartBit(report, 16);
    assert(testseg != NULL);
    assert(testseg->InputType == MAP_TYPE_SCALE);
    assert(testseg->OutputControl == MAP_MOUSE_X);
    assert(testseg->OutputChannel == MAP_MOUSE);

    testseg = FindSegByStartBit(report, 28);
    assert(testseg != NULL);
    assert(testseg->InputType == MAP_TYPE_SCALE);
    assert(testseg->OutputControl == MAP_MOUSE_Y);
    assert(testseg->OutputChannel == MAP_MOUSE);

    testseg = FindSegByStartBit(report, 40);
    assert(testseg != NULL);
    assert(testseg->InputType == MAP_TYPE_SCALE);
    assert(testseg->OutputControl == MAP_MOUSE_WHEEL);
    assert(testseg->OutputChannel == MAP_MOUSE);
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

    TestG304();
   // TestStandardKeyboard();




    printf("Parser tests passed\n");

    for (;;)
    {
        //printf("v");
    }
}