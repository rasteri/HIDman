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

__xdata uint8_t KeyboardTestDataD[] = { 0, 0, 4, 5, 6, 7, 8, 9 };
__xdata uint8_t KeyboardTestDataU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

__xdata uint8_t MouseTestData[] = { 0, 0, 0 };

//__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0x21, 0x43, 0x65, 0x00 };

__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
__xdata uint8_t FakeG304TestData2[] = { 0x02, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00  };
__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x01 , 0x80 , 0x04 , 0x00 , 0x00 };
//__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x7F , 0x30 , 0x00 , 0x00 , 0x00  };
//__xdata uint8_t FakeG304TestData3[] = { 0x02, 0x00, 0x02 , 0xB0 , 0xFF , 0x00 , 0x00 };
// Who says you can't do dependency injection in C
__xdata bool MenuActive = 0;

// mask off the number of bits
__code uint32_t bitMasks32[] = {
    0x0000, 
    0x0001, 0x0003, 0x0007, 0x000f, 0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0fFF, 0x1fFF, 0x3fFF, 0x7fFF, 0xFFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000fFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0fFFFFFF, 0x1fFFFFFF, 0x3fFFFFFF, 0x7fFFFFFF, 0xFFFFFFFF
 };

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

__xdata volatile uint16_t SoftWatchdog = 0;

void Menu_Press_Key(uint8_t key)
{

}

void SetKeyboardLedStatusFromPS2(UINT8 ps2led)
{

}
extern __code unsigned char KEY_SET2_A_MAKE[];
extern __code unsigned char KEY_SET2_B_MAKE[];
extern __code unsigned char KEY_SET2_C_MAKE[];
extern __code unsigned char KEY_SET2_D_MAKE[];
extern __code unsigned char KEY_SET2_E_MAKE[];
extern __code unsigned char KEY_SET2_F_MAKE[];
extern __code unsigned char KEY_SET2_A_BREAK[];
extern __code unsigned char KEY_SET2_B_BREAK[];
extern __code unsigned char KEY_SET2_C_BREAK[];
extern __code unsigned char KEY_SET2_D_BREAK[];
extern __code unsigned char KEY_SET2_E_BREAK[];
extern __code unsigned char KEY_SET2_F_BREAK[];

uint8_t * GetNextChunk(){
    if (ports[PORT_KEY].sendBuffEnd == ports[PORT_MOUSE].sendBuffStart)
        return NULL;
    uint8_t * chunk = ports[PORT_KEY].sendBuff.chunky[ports[PORT_KEY].sendBuffStart];
    ports[PORT_KEY].sendBuffStart = (ports[PORT_KEY].sendBuffStart + 1) & 0x3F;
    return chunk;
}

uint8_t * GetNextChonk(){
    if (ports[PORT_MOUSE].sendBuffEnd == ports[PORT_MOUSE].sendBuffStart)
        return NULL;
    uint8_t * chonk = ports[PORT_MOUSE].sendBuff.chonky[ports[PORT_MOUSE].sendBuffStart];
    ports[PORT_MOUSE].sendBuffStart = (ports[PORT_MOUSE].sendBuffStart + 1) & 0x07;
    return chonk;
}

void TestStandardKeyboard(void){

    InitPS2Ports();

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);

    assert(UsbDev.InterfaceNum == 2);

    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, StandardKeyboardDescriptor, 63));
    assert(!pInterface->usesReports);


    __xdata HID_REPORT *report = (__xdata HID_REPORT *)ListGetData(pInterface->Reports, 0);

    assert(report->length == 64);
    assert(report->appUsagePage == REPORT_USAGE_PAGE_GENERIC);
    assert(report->appUsage == REPORT_USAGE_KEYBOARD);

    // modifyers
    __xdata HID_SEG * testseg = FindSegByStartBit(report, 0);

    assert(testseg != NULL);
    assert(testseg->InputType == MAP_TYPE_BITFIELD);
    assert(testseg->OutputChannel == MAP_KEYBOARD);
    assert(testseg->OutputControl == 0xE0);

    // scancode array
    for( uint8_t x = 16; x < 64; x += 8){
        testseg = FindSegByStartBit(report, x);
        assert(testseg != NULL);
        assert(testseg->OutputChannel == MAP_KEYBOARD);
        assert(testseg->InputType == MAP_TYPE_ARRAY);
    }

    printf("Standard Keyboard Parser Test Passed\n");

    // put a bunch of stuff in the buffer (should be 60 entries)
    int c;
    for (int c = 0; c < 5; c++){
        assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataD));
        assert (ParseReport(pInterface, 8 * 8, KeyboardTestDataU));
    }

    // make sure it comes out the other end
    for (int c = 0; c < 5; c++){
        assert (GetNextChunk() == KEY_SET2_A_MAKE);
        assert (GetNextChunk() == KEY_SET2_B_MAKE);
        assert (GetNextChunk() == KEY_SET2_C_MAKE);
        assert (GetNextChunk() == KEY_SET2_D_MAKE);
        assert (GetNextChunk() == KEY_SET2_E_MAKE);
        assert (GetNextChunk() == KEY_SET2_F_MAKE);

        assert (GetNextChunk() == KEY_SET2_A_BREAK);
        assert (GetNextChunk() == KEY_SET2_B_BREAK);
        assert (GetNextChunk() == KEY_SET2_C_BREAK);
        assert (GetNextChunk() == KEY_SET2_D_BREAK);
        assert (GetNextChunk() == KEY_SET2_E_BREAK);
        assert (GetNextChunk() == KEY_SET2_F_BREAK);
    }

    printf("End-to-end Standard Keyboard Test Passed\n");

    //DumpHID(pInterface);
}


void TestStandardMouse(void){

    InitPS2Ports();
    InitMice();

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);
    assert(!InterfaceParseReportDescriptor(pInterface, StandardMouseDescriptor, 50));

    // put a bunch of stuff in the buffer (should be 60 entries)
    int c;
    for (int c = 0; c < 255; c++){
        MouseTestData[0];
        MouseTestData[1] = (uint8_t)rand();
        MouseTestData[2] = (uint8_t)rand();
        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        uint8_t * chonk = GetNextChonk();
        //printf("%X %X - %X %X %X %X\n", MouseTestData[1], MouseTestData[2], chonk[0], chonk[1], chonk[2], (uint8_t)(255-chonk[3])+(uint8_t)1);
        assert (chonk != NULL);
        assert (MouseTestData[1] == chonk[2]);
        assert (MouseTestData[2] == (uint8_t)(( 255 - chonk[3]) + 1));
        // todo, check sign bits and buttons
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

void TestFakeG304(){

    InitTest(&UsbDev, FakeG304DeviceDescriptor, 18, FakeG304ConfigDescriptor, 34);

    assert(UsbDev.InterfaceNum == 1);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, FakeG304ReportDescriptor, 105));

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

    printf("Fake G304 Parser Test Passed\n");
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
    TestSegExtracts();  
    TestFakeG304();

    printf("ALL TESTS PASSED\n");

    for (;;)
    {
        //printf("v");
    }
}