#include "defs.h"
#include "type.h"



__code uint8_t QMKKeyboardDeviceDescriptor[] = {
0x12,        // bLength
0x01,        // bDescriptorType (Device)
0x00, 0x02,  // bcdUSB 2.00
0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
0x00,        // bDeviceSubClass 
0x00,        // bDeviceProtocol 
0x40,        // bMaxPacketSize0 64
0xED, 0xFE,  // idVendor 0xFEED
0x65, 0x64,  // idProduct 0x6465
0x01, 0x00,  // bcdDevice 0.01
0x01,        // iManufacturer (String Index)
0x02,        // iProduct (String Index)
0x00,        // iSerialNumber (String Index)
0x01,        // bNumConfigurations 1

// 18 bytes
};

__code uint8_t QMKKeyboardConfigDescriptor[] = {
0x09,        //   bLength
0x02,        //   bDescriptorType (Configuration)
0x3B, 0x00,  //   wTotalLength 59
0x02,        //   bNumInterfaces 2
0x01,        //   bConfigurationValue
0x00,        //   iConfiguration (String Index)
0xA0,        //   bmAttributes Remote Wakeup
0xFA,        //   bMaxPower 500mA

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
0x11, 0x01,  //   bcdHID 1.11
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x44, 0x00,  //   wDescriptorLength[0] 68

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x81,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x01,        //   bInterval 1 (unit depends on device speed)

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
0x11, 0x01,  //   bcdHID 1.11
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x32, 0x00,  //   wDescriptorLength[0] 50

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x82,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x20, 0x00,  //   wMaxPacketSize 32
0x01,        //   bInterval 1 (unit depends on device speed)

// 59 bytes

};

__code uint8_t QMKKeyboardReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x80,        // Usage (Sys Control)
0xA1, 0x01,        // Collection (Application)
0x85, 0x03,        //   Report ID (3)
0x19, 0x01,        //   Usage Minimum (Pointer)
0x2A, 0xB7, 0x00,  //   Usage Maximum (Sys Display LCD Autoscale)
0x15, 0x01,        //   Logical Minimum (1)
0x26, 0xB7, 0x00,  //   Logical Maximum (183)
0x95, 0x01,        //   Report Count (1)
0x75, 0x10,        //   Report Size (16)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0x05, 0x0C,        // Usage Page (Consumer)
0x09, 0x01,        // Usage (Consumer Control)
0xA1, 0x01,        // Collection (Application)
0x85, 0x04,        //   Report ID (4)
0x19, 0x01,        //   Usage Minimum (Consumer Control)
0x2A, 0xA0, 0x02,  //   Usage Maximum (0x02A0)
0x15, 0x01,        //   Logical Minimum (1)
0x26, 0xA0, 0x02,  //   Logical Maximum (672)
0x95, 0x01,        //   Report Count (1)
0x75, 0x10,        //   Report Size (16)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x06,        // Usage (Keyboard)
0xA1, 0x01,        // Collection (Application)
0x85, 0x06,        //   Report ID (6)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0xE0,        //   Usage Minimum (0xE0)
0x29, 0xE7,        //   Usage Maximum (0xE7)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x95, 0x08,        //   Report Count (8)
0x75, 0x01,        //   Report Size (1)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0x00,        //   Usage Minimum (0x00)
0x29, 0xEF,        //   Usage Maximum (0xEF)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x95, 0xF0,        //   Report Count (-16)
0x75, 0x01,        //   Report Size (1)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x08,        //   Usage Page (LEDs)
0x19, 0x01,        //   Usage Minimum (Num Lock)
0x29, 0x05,        //   Usage Maximum (Kana)
0x95, 0x05,        //   Report Count (5)
0x75, 0x01,        //   Report Size (1)
0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x01,        //   Report Count (1)
0x75, 0x03,        //   Report Size (3)
0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection

// 109 bytes


};

__xdata uint8_t QMKKeyboardReportPressA[] = {
0x06,        
0x00,        
0x10, 0x00, 0x00, 0x00, 
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00        
// 32 bytes
};


__xdata uint8_t QMKKeyboardReportReleaseA[] = {
0x06,        
0x00,        
0x00, 
0x00, 
0x00, 
0x00, 
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00,        
0x00        
// 32 bytes
};


__code uint8_t KeychronWirelessKeyboardDeviceDescriptor[] = {
0x12,        // bLength
0x01,        // bDescriptorType (Device)
0x10, 0x01,  // bcdUSB 1.10
0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
0x00,        // bDeviceSubClass 
0x00,        // bDeviceProtocol 
0x08,        // bMaxPacketSize0 8
0x34, 0x34,  // idVendor 0x3434
0x30, 0xD0,  // idProduct 0xD030
0x04, 0xD2,  // bcdDevice 342.04
0x01,        // iManufacturer (String Index)
0x02,        // iProduct (String Index)
0x00,        // iSerialNumber (String Index)
0x01,        // bNumConfigurations 1

// 18 bytes

};

__code uint8_t KeychronWirelessKeyboardConfigDescriptor[] = {
0x09,        //     bLength
0x02,        //     bDescriptorType (Configuration)
0x5B, 0x00,  //     wTotalLength 91
0x03,        //     bNumInterfaces 3
0x01,        //     bConfigurationValue
0x00,        //     iConfiguration (String Index)
0xA0,        //     bmAttributes Remote Wakeup
0x32,        //     bMaxPower 100mA

0x09,        //     bLength
0x04,        //     bDescriptorType (Interface)
0x00,        //     bInterfaceNumber 0
0x00,        //     bAlternateSetting
0x01,        //     bNumEndpoints 1
0x03,        //     bInterfaceClass
0x01,        //     bInterfaceSubClass
0x02,        //     bInterfaceProtocol
0x00,        //     iInterface (String Index)

0x09,        //     bLength
0x21,        //     bDescriptorType (HID)
0x11, 0x01,  //     bcdHID 1.11
0x21,        //     bCountryCode
0x01,        //     bNumDescriptors
0x22,        //     bDescriptorType[0] (HID)
0x51, 0x00,  //     wDescriptorLength[0] 81

0x07,        //     bLength
0x05,        //     bDescriptorType (Endpoint)
0x82,        //     bEndpointAddress (IN/D2H)
0x03,        //     bmAttributes (Interrupt)
0x40, 0x00,  //     wMaxPacketSize 64
0x01,        //     bInterval 1 (unit depends on device speed)

0x09,        //     bLength
0x04,        //     bDescriptorType (Interface)
0x01,        //     bInterfaceNumber 1
0x00,        //     bAlternateSetting
0x02,        //     bNumEndpoints 2
0x03,        //     bInterfaceClass
0x00,        //     bInterfaceSubClass
0x00,        //     bInterfaceProtocol
0x00,        //     iInterface (String Index)

0x09,        //     bLength
0x21,        //     bDescriptorType (HID)
0x11, 0x01,  //     bcdHID 1.11
0x21,        //     bCountryCode
0x01,        //     bNumDescriptors
0x22,        //     bDescriptorType[0] (HID)
0x73, 0x00,  //     wDescriptorLength[0] 115

0x07,        //     bLength
0x05,        //     bDescriptorType (Endpoint)
0x84,        //     bEndpointAddress (IN/D2H)
0x03,        //     bmAttributes (Interrupt)
0x40, 0x00,  //     wMaxPacketSize 64
0x01,        //     bInterval 1 (unit depends on device speed)

0x07,        //     bLength
0x05,        //     bDescriptorType (Endpoint)
0x05,        //     bEndpointAddress (OUT/H2D)
0x03,        //     bmAttributes (Interrupt)
0x40, 0x00,  //     wMaxPacketSize 64
0x01,        //     bInterval 1 (unit depends on device speed)

0x09,        //     bLength
0x04,        //     bDescriptorType (Interface)
0x02,        //     bInterfaceNumber 2
0x00,        //     bAlternateSetting
0x01,        //     bNumEndpoints 1
0x03,        //     bInterfaceClass
0x01,        //     bInterfaceSubClass
0x01,        //     bInterfaceProtocol
0x00,        //     iInterface (String Index)

0x09,        //     bLength
0x21,        //     bDescriptorType (HID)
0x11, 0x01,  //     bcdHID 1.11
0x21,        //     bCountryCode
0x01,        //     bNumDescriptors
0x22,        //     bDescriptorType[0] (HID)
0xA4, 0x00,  //     wDescriptorLength[0] 164

0x07,        //     bLength
0x05,        //     bDescriptorType (Endpoint)
0x81,        //     bEndpointAddress (IN/D2H)
0x03,        //     bmAttributes (Interrupt)
0x20, 0x00,  //     wMaxPacketSize 32
0x01,        //     bInterval 1 (unit depends on device speed)

// 91 bytes

// best guess: USB Standard Descriptor
};

__code uint8_t KeychronWirelessKeyboardReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x06,        // Usage (Keyboard)
0xA1, 0x01,        // Collection (Application)
0x85, 0x01,        //   Report ID (1)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0xE0,        //   Usage Minimum (0xE0)
0x29, 0xE7,        //   Usage Maximum (0xE7)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x08,        //   Report Count (8)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x01,        //   Report Count (1)
0x75, 0x08,        //   Report Size (8)
0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x05,        //   Report Count (5)
0x75, 0x01,        //   Report Size (1)
0x05, 0x08,        //   Usage Page (LEDs)
0x19, 0x01,        //   Usage Minimum (Num Lock)
0x29, 0x05,        //   Usage Maximum (Kana)
0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x01,        //   Report Count (1)
0x75, 0x03,        //   Report Size (3)
0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x06,        //   Report Count (6)
0x75, 0x08,        //   Report Size (8)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0xF1, 0x00,  //   Logical Maximum (241)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0x00,        //   Usage Minimum (0x00)
0x2A, 0xF1, 0x00,  //   Usage Maximum (0xF1)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
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
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x80,        // Usage (Sys Control)
0xA1, 0x01,        // Collection (Application)
0x85, 0x06,        //   Report ID (6)
0x09, 0x82,        //   Usage (Sys Sleep)
0x09, 0x81,        //   Usage (Sys Power Down)
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
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x06,        // Usage (Keyboard)
0xA1, 0x01,        // Collection (Application)
0x85, 0x0C,        //   Report ID (12)
0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x19, 0xE0,        //   Usage Minimum (0xE0)
0x29, 0xE7,        //   Usage Maximum (0xE7)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x75, 0x01,        //   Report Size (1)
0x95, 0x08,        //   Report Count (8)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
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

/*__code uint8_t KeychronWirelessReportPressA[] = {
0x0C,        
0x00,        
0x10,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__code uint8_t KeychronWirelessReportReleaseA[] = {
0x0C,        
0x00,        
0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__code uint8_t KeychronWirelessReportPressA[] = {
0x01,        
0x00,0x00,0x04,0x00,0x00,0x00,0x00
};

__code uint8_t KeychronWirelessReportReleaseA[] = {
0x01,        
0x00,0x00,0x00,0x00,0x00,0x00,0x00
};*/


__code uint8_t MiniKeyboardTouchpadDeviceDescriptor[] = {
0x12,        // bLength
0x01,        // bDescriptorType (Device)
0x10, 0x01,  // bcdUSB 1.10
0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
0x00,        // bDeviceSubClass 
0x00,        // bDeviceProtocol 
0x08,        // bMaxPacketSize0 8
0x0C, 0x40,  // idVendor 0x400C
0x7A, 0x10,  // idProduct 0x107A
0x10, 0x01,  // bcdDevice 2.10
0x00,        // iManufacturer (String Index)
0x01,        // iProduct (String Index)
0x00,        // iSerialNumber (String Index)
0x01,        // bNumConfigurations 1

// 18 bytes
};

__code uint8_t MiniKeyboardTouchpadConfigDescriptor[] = {
0x09,        //   bLength
0x02,        //   bDescriptorType (Configuration)
0x3B, 0x00,  //   wTotalLength 59
0x02,        //   bNumInterfaces 2
0x01,        //   bConfigurationValue
0x00,        //   iConfiguration (String Index)
0xA0,        //   bmAttributes Remote Wakeup
0x19,        //   bMaxPower 50mA

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
0x41, 0x00,  //   wDescriptorLength[0] 65

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x81,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x08,        //   bInterval 8 (unit depends on device speed)

0x09,        //   bLength
0x04,        //   bDescriptorType (Interface)
0x01,        //   bInterfaceNumber 1
0x00,        //   bAlternateSetting
0x01,        //   bNumEndpoints 1
0x03,        //   bInterfaceClass
0x01,        //   bInterfaceSubClass
0x02,        //   bInterfaceProtocol
0x00,        //   iInterface (String Index)

0x09,        //   bLength
0x21,        //   bDescriptorType (HID)
0x10, 0x01,  //   bcdHID 1.10
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x77, 0x00,  //   wDescriptorLength[0] 119

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x82,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x08,        //   bInterval 8 (unit depends on device speed)

// 59 bytes
};


__code uint8_t MiniKeyboardTouchpadReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x02,        // Usage (Mouse)
0xA1, 0x01,        // Collection (Application)
0x85, 0x02,        //   Report ID (2)
0x09, 0x01,        //   Usage (Pointer)
0xA1, 0x00,        //   Collection (Physical)
0x05, 0x09,        //     Usage Page (Button)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x05,        //     Usage Maximum (0x05)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x95, 0x05,        //     Report Count (5)
0x75, 0x01,        //     Report Size (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x01,        //     Report Count (1)
0x75, 0x03,        //     Report Size (3)
0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
0x09, 0x30,        //     Usage (X)
0x09, 0x31,        //     Usage (Y)
0x09, 0x38,        //     Usage (Wheel)
0x15, 0x80,        //     Logical Minimum (-128)
0x25, 0x7F,        //     Logical Maximum (127)
0x75, 0x08,        //     Report Size (8)
0x95, 0x03,        //     Report Count (3)
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
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0x3C, 0x04,  //   Logical Maximum (1084)
0x19, 0x00,        //   Usage Minimum (Unassigned)
0x2A, 0x3C, 0x04,  //   Usage Maximum (0x043C)
0x95, 0x01,        //   Report Count (1)
0x75, 0x10,        //   Report Size (16)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x80,        // Usage (Sys Control)
0xA1, 0x01,        // Collection (Application)
0x85, 0x03,        //   Report ID (3)
0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
0x19, 0x81,        //   Usage Minimum (Sys Power Down)
0x29, 0x83,        //   Usage Maximum (Sys Wake Up)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x01,        //   Logical Maximum (1)
0x95, 0x03,        //   Report Count (3)
0x75, 0x01,        //   Report Size (1)
0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x01,        //   Report Count (1)
0x75, 0x05,        //   Report Size (5)
0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection

// 119 bytes

};

__code uint8_t Mouse16BitTestData[] = { 00, 00, 0xD0, 0x8A, 0x30, 0x75, 0xEC, 00 };


__code uint8_t Mouse16BitDeviceDescriptor[] = {
    0x12,        // bLength
0x01,        // bDescriptorType (Device)
0x00, 0x02,  // bcdUSB 2.00
0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
0x00,        // bDeviceSubClass 
0x00,        // bDeviceProtocol 
0x40,        // bMaxPacketSize0 64
0x6D, 0x04,  // idVendor 0x046D
0x8B, 0xC0,  // idProduct 0xC08B
0x03, 0x27,  // bcdDevice 66.03
0x01,        // iManufacturer (String Index)
0x02,        // iProduct (String Index)
0x03,        // iSerialNumber (String Index)
0x01,        // bNumConfigurations 1

// 18 bytes

};

__code uint8_t Mouse16BitConfigDescriptor[] = {
    0x09,        //   bLength
0x02,        //   bDescriptorType (Configuration)
0x3B, 0x00,  //   wTotalLength 59
0x02,        //   bNumInterfaces 2
0x01,        //   bConfigurationValue
0x04,        //   iConfiguration (String Index)
0xA0,        //   bmAttributes Remote Wakeup
0x96,        //   bMaxPower 300mA

0x09,        //   bLength
0x04,        //   bDescriptorType (Interface)
0x00,        //   bInterfaceNumber 0
0x00,        //   bAlternateSetting
0x01,        //   bNumEndpoints 1
0x03,        //   bInterfaceClass
0x01,        //   bInterfaceSubClass
0x02,        //   bInterfaceProtocol
0x00,        //   iInterface (String Index)

0x09,        //   bLength
0x21,        //   bDescriptorType (HID)
0x11, 0x01,  //   bcdHID 1.11
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x43, 0x00,  //   wDescriptorLength[0] 67

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x81,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x08, 0x00,  //   wMaxPacketSize 8
0x01,        //   bInterval 1 (unit depends on device speed)

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
0x11, 0x01,  //   bcdHID 1.11
0x00,        //   bCountryCode
0x01,        //   bNumDescriptors
0x22,        //   bDescriptorType[0] (HID)
0x97, 0x00,  //   wDescriptorLength[0] 151

0x07,        //   bLength
0x05,        //   bDescriptorType (Endpoint)
0x82,        //   bEndpointAddress (IN/D2H)
0x03,        //   bmAttributes (Interrupt)
0x14, 0x00,  //   wMaxPacketSize 20
0x01,        //   bInterval 1 (unit depends on device speed)

// 59 bytes
};


__code uint8_t Mouse16BitReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x02,        // Usage (Mouse)
0xA1, 0x01,        // Collection (Application)
0x09, 0x01,        //   Usage (Pointer)
0xA1, 0x00,        //   Collection (Physical)
0x05, 0x09,        //     Usage Page (Button)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x10,        //     Usage Maximum (0x10)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x95, 0x10,        //     Report Count (16)
0x75, 0x01,        //     Report Size (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
0x16, 0x01, 0x80,  //     Logical Minimum (-32767)
0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
0x75, 0x10,        //     Report Size (16)
0x95, 0x02,        //     Report Count (2)
0x09, 0x30,        //     Usage (X)
0x09, 0x31,        //     Usage (Y)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0x15, 0x81,        //     Logical Minimum (-127)
0x25, 0x7F,        //     Logical Maximum (127)
0x75, 0x08,        //     Report Size (8)
0x95, 0x01,        //     Report Count (1)
0x09, 0x38,        //     Usage (Wheel)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x0C,        //     Usage Page (Consumer)
0x0A, 0x38, 0x02,  //     Usage (AC Pan)
0x95, 0x01,        //     Report Count (1)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xC0,              // End Collection

// 67 bytes

};



__code uint8_t KovaReportDescriptor[] = {
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x02,        // Usage (Mouse)
0xA1, 0x01,        // Collection (Application)
0x85, 0x01,        //   Report ID (1)
0x09, 0x01,        //   Usage (Pointer)
0xA1, 0x00,        //   Collection (Physical)
0x05, 0x09,        //     Usage Page (Button)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x05,        //     Usage Maximum (0x05)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x95, 0x05,        //     Report Count (5)
0x75, 0x01,        //     Report Size (1)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x01,        //     Report Count (1)
0x75, 0x03,        //     Report Size (3)
0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
0x09, 0x30,        //     Usage (X)
0x09, 0x31,        //     Usage (Y)
0x16, 0x00, 0x80,  //     Logical Minimum (-32768)
0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
0x95, 0x02,        //     Report Count (2)
0x75, 0x10,        //     Report Size (16)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0x09, 0x38,        //     Usage (Wheel)
0x15, 0x81,        //     Logical Minimum (-127)
0x25, 0x7F,        //     Logical Maximum (127)
0x95, 0x01,        //     Report Count (1)
0x75, 0x08,        //     Report Size (8)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0x05, 0x0C,        //     Usage Page (Consumer)
0x0A, 0x38, 0x02,  //     Usage (AC Pan)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xC0,              // End Collection
0x05, 0x0C,        // Usage Page (Consumer)
0x09, 0x01,        // Usage (Consumer Control)
0xA1, 0x01,        // Collection (Application)
0x85, 0x02,        //   Report ID (2)
0x19, 0x00,        //   Usage Minimum (Unassigned)
0x2A, 0x3C, 0x02,  //   Usage Maximum (AC Format)
0x15, 0x00,        //   Logical Minimum (0)
0x26, 0x3C, 0x02,  //   Logical Maximum (572)
0x95, 0x01,        //   Report Count (1)
0x75, 0x10,        //   Report Size (16)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0x05, 0x0A,        // Usage Page (Ordinal)
0x09, 0x00,        // Usage (0x00)
0xA1, 0x01,        // Collection (Application)
0x85, 0x03,        //   Report ID (3)
0x19, 0x00,        //   Usage Minimum (0x00)
0x29, 0x00,        //   Usage Maximum (0x00)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x00,        //   Logical Maximum (0)
0x95, 0x04,        //   Report Count (4)
0x75, 0x08,        //   Report Size (8)
0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection
0x05, 0x0B,        // Usage Page (Telephony)
0x09, 0x00,        // Usage (Unassigned)
0xA1, 0x01,        // Collection (Application)
0x85, 0x04,        //   Report ID (4)
0x19, 0x00,        //   Usage Minimum (Unassigned)
0x29, 0x00,        //   Usage Maximum (Unassigned)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x00,        //   Logical Maximum (0)
0x95, 0x02,        //   Report Count (2)
0x75, 0x08,        //   Report Size (8)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x05,        //   Report ID (5)
0x95, 0x03,        //   Report Count (3)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x06,        //   Report ID (6)
0x95, 0x1B,        //   Report Count (27)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x07,        //   Report ID (7)
0x95, 0x4A,        //   Report Count (74)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x08,        //   Report ID (8)
0x96, 0x01, 0x04,  //   Report Count (1025)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x09,        //   Report ID (9)
0x95, 0x07,        //   Report Count (7)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x0A,        //   Report ID (10)
0x95, 0x07,        //   Report Count (7)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x0C,        //   Report ID (12)
0x95, 0x03,        //   Report Count (3)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x0D,        //   Report ID (13)
0x96, 0x03, 0x04,  //   Report Count (1027)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x0E,        //   Report ID (14)
0x95, 0x02,        //   Report Count (2)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x0F,        //   Report ID (15)
0x95, 0x05,        //   Report Count (5)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x10,        //   Report ID (16)
0x95, 0x0F,        //   Report Count (15)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x1A,        //   Report ID (26)
0x96, 0x04, 0x04,  //   Report Count (1028)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x1B,        //   Report ID (27)
0x96, 0x01, 0x04,  //   Report Count (1025)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x85, 0x1C,        //   Report ID (28)
0x95, 0x02,        //   Report Count (2)
0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              // End Collection

// 232 bytes
};

