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
#include "scancode.h"



__code uint8_t KeyChronDeviceDescriptor[] = {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x00, 0x02,  // bcdUSB 2.00
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x08,        // bMaxPacketSize0 8
    0x34, 0x34,  // idVendor 0x3434
    0x30, 0xD0,  // idProduct 0xD030
    0x00, 0xD3,  // bcdDevice 344.00
    0x01,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

    // 18 bytes
};

__code uint8_t KeyChronConfigDescriptor[] = {
    0x09,        // bLength
    0x02,        // bDescriptorType (Configuration)
    0x74, 0x00,  // wTotalLength 116
    0x04,        // bNumInterfaces 4
    0x01,        // bConfigurationValue
    0x00,        // iConfiguration (String Index)
    0xA0,        // bmAttributes Remote Wakeup
    0x32,        // bMaxPower 100mA

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x00,        // bInterfaceNumber 0
    0x00,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x03,        // bInterfaceClass
    0x01,        // bInterfaceSubClass
    0x02,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x11, 0x01,  // bcdHID 1.11
    0x21,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x51, 0x00,  // wDescriptorLength[0] 81

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x82,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x01,        // bInterval 1 (unit depends on device speed)

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x01,        // bInterfaceNumber 1
    0x00,        // bAlternateSetting
    0x02,        // bNumEndpoints 2
    0x03,        // bInterfaceClass
    0x01,        // bInterfaceSubClass
    0x01,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x11, 0x01,  // bcdHID 1.11
    0x21,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0xA7, 0x00,  // wDescriptorLength[0] 167

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x84,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x01,        // bInterval 1 (unit depends on device speed)

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x05,        // bEndpointAddress (OUT/H2D)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x01,        // bInterval 1 (unit depends on device speed)

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x02,        // bInterfaceNumber 2
    0x00,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x03,        // bInterfaceClass
    0x01,        // bInterfaceSubClass
    0x01,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x11, 0x01,  // bcdHID 1.11
    0x21,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0xA4, 0x00,  // wDescriptorLength[0] 164

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x87,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x20, 0x00,  // wMaxPacketSize 32
    0x01,        // bInterval 1 (unit depends on device speed)

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x03,        // bInterfaceNumber 3
    0x00,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x03,        // bInterfaceClass
    0x00,        // bInterfaceSubClass
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x11, 0x01,  // bcdHID 1.11
    0x21,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x22, 0x00,  // wDescriptorLength[0] 34

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x83,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x20, 0x00,  // wMaxPacketSize 32
    0x01,        // bInterval 1 (unit depends on device speed)

    // 116 bytes

};

// Interface 1, Mouse endpoint
__code uint8_t KeyChronReportDescriptor0[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x03,        //   Report ID (3)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x05,        //     Usage Maximum (0x05)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x05,        //     Report Count (5)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x03,        //     Report Size (3)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x16, 0x01, 0x80,  //     Logical Minimum (-32767)
    0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    0x75, 0x10,        //     Report Size (16)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,        //     Usage (Wheel)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0C,        //     Usage Page (Consumer)
    0x0A, 0x38, 0x02,  //     Usage (AC Pan)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection

    // 81 bytes
};

// Interface 2, another mouse and some other stuff?
__code uint8_t KeyChronReportDescriptor1[] = {
    0x06, 0xB5, 0xFF,  // Usage Page (Vendor Defined 0xFFB5)
    0x09, 0x01,        // Usage (0x01)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0xB5,        //   Report ID (-75)
    0x09, 0x02,        //   Usage (0x02)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x07,        //   Report Count (7)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x02,        //   Usage (0x02)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x07,        //   Report Count (7)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (2)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x08,        //     Usage Maximum (0x08)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x95, 0x08,        //     Report Count (8)
    0x75, 0x01,        //     Report Size (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x16, 0x01, 0xF8,  //     Logical Minimum (-2047)
    0x26, 0xFF, 0x07,  //     Logical Maximum (2047)
    0x75, 0x0C,        //     Report Size (12)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,        //     Usage (Wheel)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0C,        //     Usage Page (Consumer)
    0x0A, 0x38, 0x02,  //     Usage (AC Pan)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x19, 0x01,        //   Usage Minimum (Consumer Control)
    0x2A, 0xFF, 0x03,  //   Usage Maximum (0x03FF)
    0x15, 0x01,        //   Logical Minimum (1)
    0x26, 0xFF, 0x03,  //   Logical Maximum (1023)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x10,        //   Report Size (16)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x80,        // Usage (Sys Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x03,        //   Report ID (3)
    0x1A, 0x81, 0x00,  //   Usage Minimum (Sys Power Down)
    0x2A, 0x83, 0x00,  //   Usage Maximum (Sys Wake Up)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x95, 0x03,        //   Report Count (3)
    0x75, 0x01,        //   Report Size (1)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,        //   Report Count (5)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
    0x06, 0xBC, 0xFF,  // Usage Page (Vendor Defined 0xFFBC)
    0x09, 0x88,        // Usage (0x88)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x04,        //   Report ID (4)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection

    // 185 bytes
};

// Interface 2, keyboard
__code uint8_t KeyChronReportDescriptor2[] = {

    // REPORT 1 - standard(ish) keyboard
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)

        // Bitfield control chars, as per usual
        0x19, 0xE0,        //   Usage Minimum (0xE0)
        0x29, 0xE7,        //   Usage Maximum (0xE7)
        0x15, 0x00,        //   Logical Minimum (0)
        0x25, 0x01,        //   Logical Maximum (1)
        0x75, 0x01,        //   Report Size (1)
        0x95, 0x08,        //   Report Count (8)
        0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

        // 8 bits of padding, I guess?
        0x95, 0x01,        //   Report Count (1)
        0x75, 0x08,        //   Report Size (8)
        0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

        // LEDs
        0x95, 0x05,        //   Report Count (5)
        0x75, 0x01,        //   Report Size (1)
        0x05, 0x08,        //   Usage Page (LEDs)
        0x19, 0x01,        //   Usage Minimum (Num Lock)
        0x29, 0x05,        //   Usage Maximum (Kana)
        0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

        //LED padding
        0x95, 0x01,        //   Report Count (1)
        0x75, 0x03,        //   Report Size (3)
        0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

        // Good ol' six value array
        0x95, 0x06,        //   Report Count (6)
        0x75, 0x08,        //   Report Size (8)
        0x15, 0x00,        //   Logical Minimum (0)
        0x26, 0xF1, 0x00,  //   Logical Maximum (241)
        0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
        0x19, 0x00,        //   Usage Minimum (0x00)
        0x2A, 0xF1, 0x00,  //   Usage Maximum (0xF1)
        0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

    0xC0,              // End Collection


    // Report 2, media controls
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (2)
    0x75, 0x10,        //   Report Size (16)
    0x95, 0x01,        //   Report Count (1)
    0x15, 0x01,        //   Logical Minimum (1)
    0x26, 0x8C, 0x03,  //   Logical Maximum (908)
    0x19, 0x01,        //   Usage Minimum (Consumer Control)
    0x2A, 0x8C, 0x03,  //   Usage Maximum (0x038C)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection


    // REPORT 6 - I dunnno, power keys I guess
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x80,        // Usage (Sys Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x06,        //   Report ID (6)
    0x09, 0x81,        //   Usage (Sys Power Down)
    0x09, 0x82,        //   Usage (Sys Sleep)
    0x09, 0x83,        //   Usage (Sys Wake Up)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x19, 0x01,        //   Usage Minimum (Pointer)
    0x29, 0x03,        //   Usage Maximum (0x03)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x03,        //   Report Count (3)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,        //   Report Count (5)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection


    // report 12 (0x0C), control keys again, and NKRO overflow
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x0C,        //   Report ID (12)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)

    // Control keys
    0x19, 0xE0,        //   Usage Minimum (0xE0)
    0x29, 0xE7,        //   Usage Maximum (0xE7)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x08,        //   Report Count (8)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

    // NKRO overflow
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0x98,        //   Usage Maximum (0x98)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x98,        //   Report Count (-104)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection

    // 164 bytes
};

// Interface 3, no idea
__code uint8_t KeyChronReportDescriptor3[] = {
    0x06, 0x60, 0xFF,  // Usage Page (Vendor Defined 0xFF60)
    0x09, 0x61,        // Usage (0x61)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x62,        //   Usage (0x62)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x63,        //   Usage (0x63)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x95, 0x20,        //   Report Count (32)
    0x75, 0x08,        //   Report Size (8)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection

    // 34 bytes
};

// This is ABCDEF
__xdata uint8_t KeyboardTestDataD[] = { 1, 0, 0, 4, 5, 6, 7, 8, 9 };
__xdata uint8_t KeyboardTestDataU[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0 };

// This should be GHIJKL
__xdata uint8_t KeyboardTestDataNKROD[] = { 0x0C, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
__xdata uint8_t KeyboardTestDataNKROU[] = { 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


__xdata uint8_t KeyboardMediaTestD[] = { 0x02, 0xE9, 0x00 }; 
__xdata uint8_t KeyboardMediaTestU[] = { 0x02, 0x00, 0x00 };

__xdata uint8_t KeyboardMediaTestD2[] = { 0x02, 0x27, 0x02 }; 
__xdata uint8_t KeyboardMediaTestU2[] = { 0x02, 0x00, 0x00 };

int main() {

    uint8_t * chonk;

    TestSetup();

    InitPS2Ports();
    InitMice();

    InitTest(&UsbDev, KeyChronDeviceDescriptor, 18, KeyChronConfigDescriptor, 116);

    assert(UsbDev.InterfaceNum == 4);
    
    // I think we're mostly interested in interface 2
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 2);

    assert(pInterface != NULL);

    assert(!InterfaceParseReportDescriptor(pInterface, KeyChronReportDescriptor2, 164));
    DumpHID(pInterface);



    uint8_t *pnt; 

/*    pnt = GetNextChunk();

    printf("%x\n",pnt[0]);
    printf("%x\n",pnt[1]);
    printf("%x\n",pnt[2]);*/

    assert (ParseReport(pInterface, 3 * 8, KeyboardMediaTestD));
    assert (ParseReport(pInterface, 3 * 8, KeyboardMediaTestU));

    assert (GetNextChunk() == KEY_SET2_VOLUP_MAKE);
    assert (GetNextChunk() == KEY_SET2_VOLUP_BREAK);

    assert (ParseReport(pInterface, 3 * 8, KeyboardMediaTestD2));
    assert (ParseReport(pInterface, 3 * 8, KeyboardMediaTestU2));

    assert (GetNextChunk() == KEY_SET2_WWWREFRESH_MAKE);
    assert (GetNextChunk() == KEY_SET2_WWWREFRESH_BREAK);

    
    assert (ParseReport(pInterface, 9 * 8, KeyboardTestDataD));
    assert (ParseReport(pInterface, 21 * 8, KeyboardTestDataNKROD));

    assert (ParseReport(pInterface, 9 * 8, KeyboardTestDataU));
    assert (ParseReport(pInterface, 21 * 8, KeyboardTestDataNKROU));

    assert (GetNextChunk() == KEY_SET2_A_MAKE);
    assert (GetNextChunk() == KEY_SET2_B_MAKE);
    assert (GetNextChunk() == KEY_SET2_C_MAKE);
    assert (GetNextChunk() == KEY_SET2_D_MAKE);
    assert (GetNextChunk() == KEY_SET2_E_MAKE);
    assert (GetNextChunk() == KEY_SET2_F_MAKE);

    assert (GetNextChunk() == KEY_SET2_G_MAKE);
    assert (GetNextChunk() == KEY_SET2_H_MAKE);
    assert (GetNextChunk() == KEY_SET2_I_MAKE);
    assert (GetNextChunk() == KEY_SET2_J_MAKE);
    assert (GetNextChunk() == KEY_SET2_K_MAKE);
    assert (GetNextChunk() == KEY_SET2_L_MAKE);

    assert (GetNextChunk() == KEY_SET2_A_BREAK);
    assert (GetNextChunk() == KEY_SET2_B_BREAK);
    assert (GetNextChunk() == KEY_SET2_C_BREAK);
    assert (GetNextChunk() == KEY_SET2_D_BREAK);
    assert (GetNextChunk() == KEY_SET2_E_BREAK);
    assert (GetNextChunk() == KEY_SET2_F_BREAK);

    assert (GetNextChunk() == KEY_SET2_G_BREAK);
    assert (GetNextChunk() == KEY_SET2_H_BREAK);
    assert (GetNextChunk() == KEY_SET2_I_BREAK);
    assert (GetNextChunk() == KEY_SET2_J_BREAK);
    assert (GetNextChunk() == KEY_SET2_K_BREAK);
    assert (GetNextChunk() == KEY_SET2_L_BREAK);



    
    //

    //assert (ParseReport(pInterface, 9 * 8, KeyboardTestDataU));

    printf("PASS\n");
    halt();
}