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

__code uint8_t CheapoKeyboardDeviceDescriptor[] = {
0x12,        // bLength
0x01,        // bDescriptorType (Device)
0x10, 0x01,  // bcdUSB 1.10
0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
0x00,        // bDeviceSubClass 
0x00,        // bDeviceProtocol 
0x08,        // bMaxPacketSize0 8
0x2C, 0x1A,  // idVendor 0x1A2C
0x24, 0x21,  // idProduct 0x2124
0x10, 0x01,  // bcdDevice 2.10
0x01,        // iManufacturer (String Index)
0x02,        // iProduct (String Index)
0x00,        // iSerialNumber (String Index)
0x01,        // bNumConfigurations 1

// 18 bytes
};

__code uint8_t CheapoKeyboardConfigDescriptor[] = {
0x09,        //   bLength
0x02,        //   bDescriptorType (Configuration)
0x3B, 0x00,  //   wTotalLength 59
0x02,        //   bNumInterfaces 2
0x01,        //   bConfigurationValue
0x00,        //   iConfiguration (String Index)
0xA0,        //   bmAttributes Remote Wakeup
0x31,        //   bMaxPower 98mA

0x09,        //   bLength
0x04,        //   bDescriptorType (Interface)
0x00,        //   bInterfaceNumber 0
0x00,        //   bAlternateSetting
0x01,        //   bNumEndpoints 1
0x03,        //   bInterfaceClass
0x01,        //   bInterfaceSubClass
0x01,        //   bInterfaceProtocol
0x00,        //   iInterface (String Index)

0x09,        //   bLength
0x21,        //   bDescriptorType (HID)
0x10, 0x01,  //   bcdHID 1.10
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x36, 0x00,  //   wDescriptorLength[0] 54

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x81,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x0A,        //   bInterval 10 (unit depends on device speed)

0x09,        //   bLength
0x04,        //   bDescriptorType (Interface)
0x01,        //   bInterfaceNumber 1
0x00,        //   bAlternateSetting
0x01,        //   bNumEndpoints 1
0x03,        //   bInterfaceClass
0x00,        //   bInterfaceSubClass
0x00,        //   bInterfaceProtocol
0x00,        //   iInterface (String Index)

0x09,        //   bLength
0x21,        //   bDescriptorType (HID)
0x10, 0x01,  //   bcdHID 1.10
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x32, 0x00,  //   wDescriptorLength[0] 50

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x82,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x0A,        //   bInterval 10 (unit depends on device speed)

// 59 bytes

};

__code uint8_t CheapoKeyboardReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x06,        // Usage (Keyboard)
0xA1, 0x01,        // Collection (Application)
0x05, 0x08,        //   Usage Page (LEDs)
0x19, 0x01,        //   Usage Minimum (Num Lock)
0x29, 0x03,        //   Usage Maximum (Scroll Lock)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x03,        //   Report Count (3)
0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x05,        //   Report Count (5)
0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0xE0,        //   Usage Minimum (0xE0)
0x29, 0xE7,        //   Usage Maximum (0xE7)
0x95, 0x08,        //   Report Count (8)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x19, 0x00,        //   Usage Minimum (0x00)
0x29, 0x91,        //   Usage Maximum (0x91)
0x26, 0xFF, 0x00,  //   Logical Maximum (255)
0x95, 0x06,        //   Report Count (6)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection

// 54 bytes


};



__xdata uint8_t KeyboardTestDataD[] = { 0, 0, 4, 5, 6, 7, 8, 9 };
__xdata uint8_t KeyboardTestDataU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

int main() {

    TestSetup();

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
    assert(report->HidSegments != NULL);

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

    printf("PASS\n");
    halt();
}
