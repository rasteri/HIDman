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

__xdata uint8_t CheapoGamepadTestDataU[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD1[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x1F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD2[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x2F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD3[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x4F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD4[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x8F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDL[] = { 0x01, 0x80, 0x80, 0x00, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDR[] = { 0x01, 0x80, 0x80, 0xFF, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDU[] = { 0x01, 0x80, 0x80, 0x7F, 0x00, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDD[] = { 0x01, 0x80, 0x80, 0x7F, 0xFF, 0x0F, 0x00, 0x00 };

__xdata uint8_t PSXAdapterTestDataU[] =  { 0x01, 0x80, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDU[] = { 0x01, 0x00, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDD[] = { 0x01, 0xFF, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDL[] = { 0x01, 0x80, 0x00, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDR[] = { 0x01, 0x80, 0xFF, 0x64, 0x80, 0x0F, 0x00, 0x00 };

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
    0x54178008, //magic
    0, //KeyboardReportMode
    0, //MouseReportMode
    0, //EnableAUXPS2
    0, //Intellimouse
    0, //XT83Keys
    1,//GameControllerAsMouse
    0, //SerialDebugOutput
    0, //USBFilter
    MODE_PS2, //KeyboardMode
    0 //MenuRateLimit
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
extern __code unsigned char KEY_SET2_UP_MAKE[];
extern __code unsigned char KEY_SET2_DOWN_MAKE[];
extern __code unsigned char KEY_SET2_LEFT_MAKE[];
extern __code unsigned char KEY_SET2_RIGHT_MAKE[];

extern __code unsigned char KEY_SET2_A_BREAK[];
extern __code unsigned char KEY_SET2_B_BREAK[];
extern __code unsigned char KEY_SET2_C_BREAK[];
extern __code unsigned char KEY_SET2_D_BREAK[];
extern __code unsigned char KEY_SET2_E_BREAK[];
extern __code unsigned char KEY_SET2_F_BREAK[];
extern __code unsigned char KEY_SET2_UP_BREAK[];
extern __code unsigned char KEY_SET2_DOWN_BREAK[];
extern __code unsigned char KEY_SET2_LEFT_BREAK[];
extern __code unsigned char KEY_SET2_RIGHT_BREAK[];

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

    assert(report != NULL);
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

void TestCheapoGamepad(void) {

    InitPS2Ports();

    InitTest(&UsbDev, CheapoGamepadDeviceDescriptor, 18, CheapoGamepadConfigDescriptor, 34);

    //assert(UsbDev.InterfaceNum == 2);

    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, CheapoGamepadReportDescriptor, 89));
    //assert(!pInterface->usesReports);


    __xdata HID_REPORT *report = (__xdata HID_REPORT *)ListGetData(pInterface->Reports, 0);

    /*assert(report->length == 64);
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

    printf("Standard Keyboard Parser Test Passed\n");*/

    int c;
    for (int c = 0; c < 1; c++) {
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataD1));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataD2));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataD3));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataD4));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataDU));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataDD));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataDL));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));

        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataDR));
        assert (ParseReport(pInterface, 8 * 8, CheapoGamepadTestDataU));
    }

    // make sure it comes out the other end
    for (int c = 0; c < 1; c++){
        assert (GetNextChunk() == KEY_SET2_A_MAKE);
        assert (GetNextChunk() == KEY_SET2_A_BREAK);

        assert (GetNextChunk() == KEY_SET2_B_MAKE);
        assert (GetNextChunk() == KEY_SET2_B_BREAK);

        assert (GetNextChunk() == KEY_SET2_C_MAKE);
        assert (GetNextChunk() == KEY_SET2_C_BREAK);

        assert (GetNextChunk() == KEY_SET2_D_MAKE);
        assert (GetNextChunk() == KEY_SET2_D_BREAK);

        assert (GetNextChunk() == KEY_SET2_UP_MAKE);
        assert (GetNextChunk() == KEY_SET2_UP_BREAK);

        assert (GetNextChunk() == KEY_SET2_DOWN_MAKE);
        assert (GetNextChunk() == KEY_SET2_DOWN_BREAK);

        assert (GetNextChunk() == KEY_SET2_LEFT_MAKE);
        assert (GetNextChunk() == KEY_SET2_LEFT_BREAK);

        assert (GetNextChunk() == KEY_SET2_RIGHT_MAKE);
        assert (GetNextChunk() == KEY_SET2_RIGHT_BREAK);
    }

    printf("End-to-end Standard Gamepad Test Passed\n");

    //DumpHID(pInterface);
}


void TestPSXAdapter(void) {

    InitPS2Ports();
    InitPresets();
    Ps2MouseSetDefaults();

    InitTest(&UsbDev, PSXAdapterDeviceDescriptor, 18, PSXAdapterConfigDescriptor, 34);

    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, PSXAdapterReportDescriptor, 202));

    __xdata HID_REPORT *report = (__xdata HID_REPORT *)ListGetData(pInterface->Reports, 0);


    int c;

    uint8_t * chonk;

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataU));

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDU));
    HandleMouse();
    chonk = GetNextChonk();


    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDD));
    HandleMouse();
    chonk = GetNextChonk();
    printf("ch %X - %X\n", chonk[2], chonk[3]);

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDL));
    HandleMouse();
    chonk = GetNextChonk();
    printf("ch %X - %X\n", chonk[2], chonk[3]);

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDR));
    HandleMouse();
    chonk = GetNextChonk();
    printf("ch %X - %X\n", chonk[2], chonk[3]);


    printf("PSX Adapter Test Passed\n");

    //DumpHID(pInterface);
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