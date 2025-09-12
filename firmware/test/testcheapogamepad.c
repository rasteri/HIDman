
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
#include "ps2.h"
#include "ps2protocol.h"
#include "testdata.h"
#include "preset.h"
#include "test.h"
#include "usbll.h"
#include "linkedlist.h"
#include "testcommon.h"

__xdata uint8_t CheapoGamepadTestDataU[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD1[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x1F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD2[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x2F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD3[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x4F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD4[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x8F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDL[] = { 0x01, 0x80, 0x80, 0x00, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDR[] = { 0x01, 0x80, 0x80, 0xFF, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDU[] = { 0x01, 0x80, 0x80, 0x7F, 0x00, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDD[] = { 0x01, 0x80, 0x80, 0x7F, 0xFF, 0x0F, 0x00, 0x00 };


__code uint8_t CheapoGamepadDeviceDescriptor[] = {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x00, 0x01,  // bcdUSB 1.00
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x08,        // bMaxPacketSize0 8
    0x10, 0x08,  // idVendor 0x0810
    0x01, 0xE5,  // idProduct 0xE501
    0x06, 0x01,  // bcdDevice 2.06
    0x00,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

    // 18 bytes

};

__code uint8_t CheapoGamepadConfigDescriptor[] = {
    0x09,        // bLength
    0x02,        // bDescriptorType (Configuration)
    0x22, 0x00,  // wTotalLength 34
    0x01,        // bNumInterfaces 1
    0x01,        // bConfigurationValue
    0x00,        // iConfiguration (String Index)
    0x80,        // bmAttributes
    0xFA,        // bMaxPower 500mA

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x00,        // bInterfaceNumber 0
    0x00,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x03,        // bInterfaceClass
    0x00,        // bInterfaceSubClass
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x10, 0x01,  // bcdHID 1.10
    0x21,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x59, 0x00,  // wDescriptorLength[0] 89

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x81,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x08, 0x00,  // wMaxPacketSize 8
    0x0A,        // bInterval 10 (unit depends on device speed)
    //34 bytes
};

__code uint8_t CheapoGamepadReportDescriptor[] = {
    0x05, 0x01,    	// Usage Page (Generic Desktop Ctrls)
0x09, 0x04,    	// Usage (Joystick)
0xA1, 0x01,    	// Collection (Application)
0x85, 0x01,    	//   Report ID (1)
0xA1, 0x02,    	//   Collection (Logical)
0x75, 0x08,    	// 	Report Size (8)
0x95, 0x02,    	// 	Report Count (2)
0x15, 0x00,    	// 	Logical Minimum (0)
0x26, 0xFF, 0x00,  // 	Logical Maximum (255)
0x35, 0x00,    	// 	Physical Minimum (0)
0x46, 0xFF, 0x00,  // 	Physical Maximum (255)
0x95, 0x02,    	// 	Report Count (2)
0x81, 0x01,    	// 	Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x02,    	// 	Report Count (2)
0x09, 0x30,    	// 	Usage (X)
0x09, 0x31,    	// 	Usage (Y)
0x81, 0x02,    	// 	Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x65, 0x00,    	// 	Unit (None)
0x75, 0x01,    	// 	Report Size (1)
0x95, 0x04,    	// 	Report Count (4)
0x25, 0x01,    	// 	Logical Maximum (1)
0x45, 0x01,    	// 	Physical Maximum (1)
0x81, 0x01,    	// 	Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x01,    	// 	Report Size (1)
0x95, 0x0A,    	// 	Report Count (10)
0x25, 0x01,    	// 	Logical Maximum (1)
0x45, 0x01,    	// 	Physical Maximum (1)
0x05, 0x09,    	// 	Usage Page (Button)
0x19, 0x01,    	// 	Usage Minimum (0x01)
0x29, 0x0A,    	// 	Usage Maximum (0x0A)
0x81, 0x02,    	// 	Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x01,    	// 	Report Size (1)
0x95, 0x02,    	// 	Report Count (2)
0x81, 0x01,    	// 	Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x06, 0x00, 0xFF,  // 	Usage Page (Vendor Defined 0xFF00)
0x75, 0x01,    	// 	Report Size (1)
0x95, 0x08,    	// 	Report Count (8)
0x25, 0x01,    	// 	Logical Maximum (1)
0x45, 0x01,    	// 	Physical Maximum (1)
0x45, 0x01,    	// 	Physical Maximum (1)
0x09, 0x01,    	// 	Usage (0x01)
0x81, 0x02,    	// 	Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,          	//   End Collection
0xC0,          	// End Collection

// 89 bytes

};

int main(void) {

    TestSetup();

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

    printf("PASS\n");
    halt();

    //DumpHID(pInterface);
}