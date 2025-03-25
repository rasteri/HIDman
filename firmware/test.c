#include <stdio.h>
#include <stdlib.h>
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
#define TESTVERBOSE

/*
Things we might want to test:
- HID report parsing. Rather than counting segs, search for segs that are the correct type, in the correct startbit, etc
- HID report processing. Can actually dig the bits out and get a sensible result at the end
- Ring buffer functions (when we write them lol)
- Presskey/releasekey actually gives a sensible result out of the ringbuffer at the end
- Test entire thing - parse a report, send some events, make sure they come out of ringbuffer at end

Probably gonna need seperate executables for different tests because of limited code space
 
*/

void UART_Init()
{
    SCON = 0x50; /* configure serial */
    TMOD = 0x20; /* configure timer */
    TH1 = 0xE6;  /* baud rate 1200 */
    TL1 = 0xE6;  /* baud rate 1200 */
    TR1 = 1;     /* enable timer */
    TI = 1;      /* enable transmitting */
    RI = 0;      /* waiting to receive */
}

static USB_HUB_PORT __xdata TestPort;

__xdata uint8_t KeyboardTestDataD[] = { 0, 0x05, 10, 20, 30, 40, 50, 0 };
__xdata uint8_t KeyboardTestDataU[] = { 0, 0x00, 0, 0, 0, 0, 0, 0 };

__xdata uint8_t KovaTestData[] = { 
0x01,       
0x00,       
0x05, 
0x00, 
0xFA, 
0xFF, 
0x00, 
0x00, 
};

__xdata uint16_t iters = 0;
__xdata bool updateiters = 0;
__xdata uint8_t bleh;
void mTimer0Interrupt(void) __interrupt(INT_NO_TMR0)
{
    if (!bleh++){
        updateiters = 1;
    }

}

bool TestDescriptors(
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen, 
    uint8_t *Report, uint16_t ReportLen,
    uint8_t ExpectedSegments){

    static __xdata INTERFACE * __xdata pInterface;

    static USB_HUB_PORT *__xdata pUsbDevice = &TestPort;

	EA = 0;	 // disable all interrupts


    InitHubPortData(pUsbDevice);

    if (!ParseDeviceDescriptor((USB_DEV_DESCR *)Dev, DevLen, pUsbDevice)) {
        printf("Can't parse Device Descriptor\n");
        return 1;
    }

    if (!ParseConfigDescriptor((USB_CFG_DESCR *)Config, ConfigLen, pUsbDevice)) {
        printf("Can't parse Config Descriptor\n");
        return 1;
    }

    #ifdef TESTVERBOSE 
        printf("Num Interfaces=%d\n", pUsbDevice->InterfaceNum);
        printf("VendorID=0x%04X,ProductID=0x%04X,bcdDevice=0x%04X\n", pUsbDevice->VendorID, pUsbDevice->ProductID, pUsbDevice->bcdDevice);
    #endif 

    for (uint8_t i = 0; i < pUsbDevice->InterfaceNum; i++)
    {
        pInterface = (__xdata INTERFACE *)ListGetData(pUsbDevice->Interfaces, i);

        if (pInterface == NULL) {
            printf("Interface %d not HID\n", i);
            continue;
        }

        #ifdef TESTVERBOSE 
            printf("\n\nInterface %d - ", i);
            printf("InterfaceClass=0x%02X - ", (UINT16)pInterface->InterfaceClass);
            printf("InterfaceProtocol=0x%02X\n", (UINT16)pInterface->InterfaceProtocol);
        #endif

		if (pInterface->InterfaceClass == USB_DEV_CLASS_HID)
		{
            if (!ParseReportDescriptor(Report, ReportLen, pInterface)) {
                printf("Can't parse Report Descriptor\n");
                return 1;
            }
            EA = 1;	 // enable all interrupts
            while(1) {
                ParseReport(pInterface, 8 * 8, KeyboardTestDataD);
                iters++;
                ParseReport(pInterface, 8 * 8, KeyboardTestDataU);
                iters++;
                if (updateiters){
                    updateiters = 0;
                    printf("i : %d\n", iters);
                    iters = 0;
                }
            }

            /*while(1){
                ParseReport(pInterface, 8 * 8, KovaTestData);
                iters++;
            }*/

            #ifdef TESTVERBOSE 
                if (DumpHID(pInterface) != ExpectedSegments){
                    printf("Expected segments wrong, not %u\n", ExpectedSegments);
                    return 1;
                }

                

            #endif
        }
    }

    return 0;

}

/*
	Func to test the sizes of various types
	Mainly just to remind me how big all the various weird pointers are
*/
#ifdef TESTVERBOSE
    #define testintsize(TP, SZ) \
        if (sizeof(TP) != SZ){ printf("type " #TP " is %d, expected " #SZ "\n", sizeof(TP));\
        return 1;}
#else
    #define testintsize(TP, SZ) \
        if (sizeof(TP) != SZ) return 1;
#endif

bool testintsizes() {

	// trad C - note ints and shorts are both 2 bytes
	testintsize(char, 1); testintsize(unsigned char, 1);
	testintsize(short, 2); testintsize(unsigned short, 2);
	testintsize(int, 2); testintsize(unsigned int, 2);
	testintsize(long, 4); testintsize(unsigned long, 4);
	testintsize(long long, 8); testintsize(unsigned long long, 8);

	// C99, this should be obvious but can't hurt to test
	testintsize(int8_t, 1); testintsize(uint8_t, 1);
	testintsize(int16_t, 2); testintsize(uint16_t, 2);
	testintsize(int32_t, 4); testintsize(uint32_t, 4);
	testintsize(int64_t, 8); testintsize(uint64_t, 8);

	// size_t is 2 bytes
	testintsize(size_t, 2);

	// stuff in xdata should work the same
	testintsize(__xdata uint8_t, 1); testintsize(__xdata uint32_t, 4);

	// generic pointers should be 24bit
	testintsize(uint8_t *, 3); testintsize(uint32_t *, 3);

	// code pointers should be 16bit
	testintsize(__code uint8_t *, 2); testintsize(__code uint32_t *, 2);

	// data pointers should be 8bit
	testintsize(__data uint8_t *, 1); testintsize(__data uint32_t *, 1);

	// idata pointers should be 8bit
	testintsize(__idata uint8_t *, 1); testintsize(__idata uint32_t *, 1);

	// xdata pointers should be 16bit
	testintsize(__xdata uint8_t *, 2); testintsize(__xdata uint32_t *, 2);

	// pdata pointers should be 8bit (they are paged 256-byte segments of xdata), page is selected by P2 (or an SFR? depends on 8051 variant)
	testintsize(__pdata uint8_t *, 1); testintsize(__pdata uint32_t *, 1);

    return 0;
}

__xdata INTERFACE funky;

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

    if (testintsizes()) printf("Integer size test failed\n");
    else printf("Integer size test passed\n");

    andyclearmem();
    InitPresets();

    //testlinkedlist();

    TestDescriptors (
        CheapoKeyboardDeviceDescriptor, 18,
        CheapoKeyboardConfigDescriptor, 59,
        StandardKeyboardDescriptor, 63,
        8
    );

    /*TestDescriptors (
        CheapoKeyboardDeviceDescriptor, 18,
        CheapoKeyboardConfigDescriptor, 59,
        KovaReportDescriptor, 232,
        8
    );*/

    /*TestDescriptors (
        PS4DeviceDescriptor, 18,
        PS4ConfigDescriptor, 225,
        PS4ReportDescriptor, 507,
        31
    );

    TestDescriptors (
        CheapoGamepadDeviceDescriptor, 18,
        CheapoGamepadConfigDescriptor, 34,
        CheapoGamepadReportDescriptor, 89,
        13
    );

    TestDescriptors (
        QMKKeyboardDeviceDescriptor, 18,
        QMKKeyboardConfigDescriptor, 59,
        QMKKeyboardReportDescriptor, 109,
        2
    );

    TestDescriptors (
        QMKKeyboardDeviceDescriptor, 18,
        QMKKeyboardConfigDescriptor, 59,
        KeychronWirelessKeyboardReportDescriptor, 164,
        10
    );

    TestDescriptors (
        CheapoKeyboardDeviceDescriptor, 18,
        CheapoKeyboardConfigDescriptor, 59,
        CheapoKeyboardReportDescriptor, 54,
        8
    );*/

   /* TestDescriptors (
        CheapoGamepadDeviceDescriptor, 18,
        CheapoGamepadConfigDescriptor, 34,
        KeychronWirelessKeyboardReportDescriptor, 164,
        8
    );*/

    /*TestDescriptors (
        MiniKeyboardTouchpadDeviceDescriptor, 18,
        MiniKeyboardTouchpadConfigDescriptor, 59,
        MiniKeyboardTouchpadReportDescriptor, 119,
        8
    );*/

    /*TestDescriptors (
        Mouse16BitDeviceDescriptor, 18,
        Mouse16BitConfigDescriptor, 59,
        Mouse16BitReportDescriptor, 67,
        10
    );*/

    printf("memused : %u\n", MemoryUsed());
    printf("memfree : %u\n", MemoryFree());

    /*Node *head = NULL;

    head = ListAdd(head, sizeof(HID_SEG));
    head->index = 1;
    ((HID_SEG *)(head->data))->startBit=0x69;

    head = ListAdd(head, sizeof(HID_SEG));
    head->index = 2;
    ((HID_SEG *)(head->data))->startBit=0x80;

    head = ListAdd(head, sizeof(HID_SEG));
    head->index = 3;
    ((HID_SEG *)(head->data))->startBit=0x08;


    if (
            ((HID_SEG *)(ListGet(head, 3)->data))->startBit != 0x08 || 
            ((HID_SEG *)(ListGet(head, 2)->data))->startBit != 0x80 || 
            ((HID_SEG *)(ListGet(head, 1)->data))->startBit != 0x69
        )
        printf("list broken\n");
    else printf("list passed\n");
    //testlinkedlist();*/

    printf("Parser tests passed\n");



    for (;;)
    {
        //printf("v");
    }
}