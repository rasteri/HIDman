#include "defs.h"
#include "type.h"

__code uint8_t PSXAdapterDeviceDescriptor[] = {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x00, 0x01,  // bcdUSB 1.00
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x08,        // bMaxPacketSize0 8
    0x10, 0x08,  // idVendor 0x0810
    0x01, 0x00,  // idProduct 0x01
    0x06, 0x01,  // bcdDevice 2.06
    0x00,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

    // 18 bytes


};

__code uint8_t PSXAdapterConfigDescriptor[] = {
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
    0xCA, 0x00,  // wDescriptorLength[0] 202

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x81,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x08, 0x00,  // wMaxPacketSize 8
    0x0A,        // bInterval 10 (unit depends on device speed)

    // 34 bytes


};

__code uint8_t PSXAdapterReportDescriptor[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,        // Usage (Joystick)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x01,        //   Report ID (1)
    0xA1, 0x02,        //   Collection (Logical)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x04,        //     Report Count (4)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x35, 0x00,        //     Physical Minimum (0)
    0x46, 0xFF, 0x00,  //     Physical Maximum (255)
    0x09, 0x32,        //     Usage (Z)
    0x09, 0x35,        //     Usage (Rz)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x04,        //     Report Size (4)
    0x95, 0x01,        //     Report Count (1)
    0x25, 0x07,        //     Logical Maximum (7)
    0x46, 0x3B, 0x01,  //     Physical Maximum (315)
    0x65, 0x14,        //     Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,        //     Usage (Hat switch)
    0x81, 0x42,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,        //     Unit (None)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x0C,        //     Report Count (12)
    0x25, 0x01,        //     Logical Maximum (1)
    0x45, 0x01,        //     Physical Maximum (1)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x0C,        //     Usage Maximum (0x0C)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //     Usage Page (Vendor Defined 0xFF00)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x08,        //     Report Count (8)
    0x25, 0x01,        //     Logical Maximum (1)
    0x45, 0x01,        //     Physical Maximum (1)
    0x09, 0x01,        //     Usage (0x01)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xA1, 0x02,        //   Collection (Logical)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x04,        //     Report Count (4)
    0x46, 0xFF, 0x00,  //     Physical Maximum (255)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x09, 0x02,        //     Usage (0x02)
    0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              //   End Collection
    0xC0,              // End Collection
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,        // Usage (Joystick)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (2)
    0xA1, 0x02,        //   Collection (Logical)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x04,        //     Report Count (4)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x35, 0x00,        //     Physical Minimum (0)
    0x46, 0xFF, 0x00,  //     Physical Maximum (255)
    0x09, 0x32,        //     Usage (Z)
    0x09, 0x35,        //     Usage (Rz)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x04,        //     Report Size (4)
    0x95, 0x01,        //     Report Count (1)
    0x25, 0x07,        //     Logical Maximum (7)
    0x46, 0x3B, 0x01,  //     Physical Maximum (315)
    0x65, 0x14,        //     Unit (System: English Rotation, Length: Centimeter)
    0x09, 0x39,        //     Usage (Hat switch)
    0x81, 0x42,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,        //     Unit (None)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x0C,        //     Report Count (12)
    0x25, 0x01,        //     Logical Maximum (1)
    0x45, 0x01,        //     Physical Maximum (1)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x0C,        //     Usage Maximum (0x0C)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //     Usage Page (Vendor Defined 0xFF00)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x08,        //     Report Count (8)
    0x25, 0x01,        //     Logical Maximum (1)
    0x45, 0x01,        //     Physical Maximum (1)
    0x09, 0x01,        //     Usage (0x01)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xA1, 0x02,        //   Collection (Logical)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x04,        //     Report Count (4)
    0x46, 0xFF, 0x00,  //     Physical Maximum (255)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x09, 0x02,        //     Usage (0x02)
    0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              //   End Collection
    0xC0,              // End Collection

    // 202 bytes

};

__code uint8_t PS4ReportDescriptor[] =
{
    0x05, 0x01,    	// Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,    	// Usage (Game Pad)
    0xA1, 0x01,    	// Collection (Application)
    0x85, 0x01,    	//   Report ID (1)
    0x09, 0x30,    	//   Usage (X)
    0x09, 0x31,    	//   Usage (Y)
    0x09, 0x32,    	//   Usage (Z)
    0x09, 0x35,    	//   Usage (Rz)
    0x15, 0x00,    	//   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,    	//   Report Size (8)
    0x95, 0x04,    	//   Report Count (4)
    0x81, 0x02,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x39,    	//   Usage (Hat switch)
    0x15, 0x00,    	//   Logical Minimum (0)
    0x25, 0x07,    	//   Logical Maximum (7)
    0x35, 0x00,    	//   Physical Minimum (0)
    0x46, 0x3B, 0x01,  //   Physical Maximum (315)
    0x65, 0x14,    	//   Unit (System: English Rotation, Length: Centimeter)
    0x75, 0x04,    	//   Report Size (4)
    0x95, 0x01,    	//   Report Count (1)
    0x81, 0x42,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
    0x65, 0x00,    	//   Unit (None)
    0x05, 0x09,    	//   Usage Page (Button)
    0x19, 0x01,    	//   Usage Minimum (0x01)
    0x29, 0x0E,    	//   Usage Maximum (0x0E)
    0x15, 0x00,    	//   Logical Minimum (0)
    0x25, 0x01,    	//   Logical Maximum (1)
    0x75, 0x01,    	//   Report Size (1)
    0x95, 0x0E,    	//   Report Count (14)
    0x81, 0x02,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x20,    	//   Usage (0x20)
    0x75, 0x06,    	//   Report Size (6)
    0x95, 0x01,    	//   Report Count (1)
    0x15, 0x00,    	//   Logical Minimum (0)
    0x25, 0x7F,    	//   Logical Maximum (127)
    0x81, 0x02,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,    	//   Usage Page (Generic Desktop Ctrls)
    0x09, 0x33,    	//   Usage (Rx)
    0x09, 0x34,    	//   Usage (Ry)
    0x15, 0x00,    	//   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,    	//   Report Size (8)
    0x95, 0x02,    	//   Report Count (2)
    0x81, 0x02,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0x09, 0x21,    	//   Usage (0x21)
    0x95, 0x36,    	//   Report Count (54)
    0x81, 0x02,    	//   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x85, 0x05,    	//   Report ID (5)
    0x09, 0x22,    	//   Usage (0x22)
    0x95, 0x1F,    	//   Report Count (31)
    0x91, 0x02,    	//   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x04,    	//   Report ID (4)
    0x09, 0x23,    	//   Usage (0x23)
    0x95, 0x24,    	//   Report Count (36)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x02,    	//   Report ID (2)
    0x09, 0x24,    	//   Usage (0x24)
    0x95, 0x24,    	//   Report Count (36)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x08,    	//   Report ID (8)
    0x09, 0x25,    	//   Usage (0x25)
    0x95, 0x03,    	//   Report Count (3)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x10,    	//   Report ID (16)
    0x09, 0x26,    	//   Usage (0x26)
    0x95, 0x04,    	//   Report Count (4)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x11,    	//   Report ID (17)
    0x09, 0x27,    	//   Usage (0x27)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x12,    	//   Report ID (18)
    0x06, 0x02, 0xFF,  //   Usage Page (Vendor Defined 0xFF02)
    0x09, 0x21,    	//   Usage (0x21)
    0x95, 0x0F,    	//   Report Count (15)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x13,    	//   Report ID (19)
    0x09, 0x22,    	//   Usage (0x22)
    0x95, 0x16,    	//   Report Count (22)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x14,    	//   Report ID (20)
    0x06, 0x05, 0xFF,  //   Usage Page (Vendor Defined 0xFF05)
    0x09, 0x20,    	//   Usage (0x20)
    0x95, 0x10,    	//   Report Count (16)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x15,    	//   Report ID (21)
    0x09, 0x21,    	//   Usage (0x21)
    0x95, 0x2C,    	//   Report Count (44)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x06, 0x80, 0xFF,  //   Usage Page (Vendor Defined 0xFF80)
    0x85, 0x80,    	//   Report ID (-128)
    0x09, 0x20,    	//   Usage (0x20)
    0x95, 0x06,    	//   Report Count (6)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x81,    	//   Report ID (-127)
    0x09, 0x21,    	//   Usage (0x21)
    0x95, 0x06,    	//   Report Count (6)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x82,    	//   Report ID (-126)
    0x09, 0x22,    	//   Usage (0x22)
    0x95, 0x05,    	//   Report Count (5)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x83,    	//   Report ID (-125)
    0x09, 0x23,    	//   Usage (0x23)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x84,    	//   Report ID (-124)
    0x09, 0x24,    	//   Usage (0x24)
    0x95, 0x04,    	//   Report Count (4)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x85,    	//   Report ID (-123)
    0x09, 0x25,    	//   Usage (0x25)
    0x95, 0x06,    	//   Report Count (6)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x86,    	//   Report ID (-122)
    0x09, 0x26,    	//   Usage (0x26)
    0x95, 0x06,    	//   Report Count (6)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x87,    	//   Report ID (-121)
    0x09, 0x27,    	//   Usage (0x27)
    0x95, 0x23,    	//   Report Count (35)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x88,    	//   Report ID (-120)
    0x09, 0x28,    	//   Usage (0x28)
    0x95, 0x22,    	//   Report Count (34)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x89,    	//   Report ID (-119)
    0x09, 0x29,    	//   Usage (0x29)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x90,    	//   Report ID (-112)
    0x09, 0x30,    	//   Usage (0x30)
    0x95, 0x05,    	//   Report Count (5)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x91,    	//   Report ID (-111)
    0x09, 0x31,    	//   Usage (0x31)
    0x95, 0x03,    	//   Report Count (3)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x92,    	//   Report ID (-110)
    0x09, 0x32,    	//   Usage (0x32)
    0x95, 0x03,    	//   Report Count (3)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0x93,    	//   Report ID (-109)
    0x09, 0x33,    	//   Usage (0x33)
    0x95, 0x0C,    	//   Report Count (12)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA0,    	//   Report ID (-96)
    0x09, 0x40,    	//   Usage (0x40)
    0x95, 0x06,    	//   Report Count (6)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA1,    	//   Report ID (-95)
    0x09, 0x41,    	//   Usage (0x41)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA2,    	//   Report ID (-94)
    0x09, 0x42,    	//   Usage (0x42)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA3,    	//   Report ID (-93)
    0x09, 0x43,    	//   Usage (0x43)
    0x95, 0x30,    	//   Report Count (48)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA4,    	//   Report ID (-92)
    0x09, 0x44,    	//   Usage (0x44)
    0x95, 0x0D,    	//   Report Count (13)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA5,    	//   Report ID (-91)
    0x09, 0x45,    	//   Usage (0x45)
    0x95, 0x15,    	//   Report Count (21)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA6,    	//   Report ID (-90)
    0x09, 0x46,    	//   Usage (0x46)
    0x95, 0x15,    	//   Report Count (21)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF0,    	//   Report ID (-16)
    0x09, 0x47,    	//   Usage (0x47)
    0x95, 0x3F,    	//   Report Count (63)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF1,    	//   Report ID (-15)
    0x09, 0x48,    	//   Usage (0x48)
    0x95, 0x3F,    	//   Report Count (63)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xF2,    	//   Report ID (-14)
    0x09, 0x49,    	//   Usage (0x49)
    0x95, 0x0F,    	//   Report Count (15)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA7,    	//   Report ID (-89)
    0x09, 0x4A,    	//   Usage (0x4A)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA8,    	//   Report ID (-88)
    0x09, 0x4B,    	//   Usage (0x4B)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xA9,    	//   Report ID (-87)
    0x09, 0x4C,    	//   Usage (0x4C)
    0x95, 0x08,    	//   Report Count (8)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAA,    	//   Report ID (-86)
    0x09, 0x4E,    	//   Usage (0x4E)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAB,    	//   Report ID (-85)
    0x09, 0x4F,    	//   Usage (0x4F)
    0x95, 0x39,    	//   Report Count (57)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAC,    	//   Report ID (-84)
    0x09, 0x50,    	//   Usage (0x50)
    0x95, 0x39,    	//   Report Count (57)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAD,    	//   Report ID (-83)
    0x09, 0x51,    	//   Usage (0x51)
    0x95, 0x0B,    	//   Report Count (11)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAE,    	//   Report ID (-82)
    0x09, 0x52,    	//   Usage (0x52)
    0x95, 0x01,    	//   Report Count (1)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xAF,    	//   Report ID (-81)
    0x09, 0x53,    	//   Usage (0x53)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xB0,    	//   Report ID (-80)
    0x09, 0x54,    	//   Usage (0x54)
    0x95, 0x3F,    	//   Report Count (63)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xB1,    	//   Report ID (-79)
    0x09, 0x55,    	//   Usage (0x55)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xB2,    	//   Report ID (-78)
    0x09, 0x56,    	//   Usage (0x56)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xE0,    	//   Report ID (-32)
    0x09, 0x57,    	//   Usage (0x57)
    0x95, 0x02,    	//   Report Count (2)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xB3,    	//   Report ID (-77)
    0x09, 0x55,    	//   Usage (0x55)
    0x95, 0x3F,    	//   Report Count (63)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x85, 0xB4,    	//   Report ID (-76)
    0x09, 0x55,    	//   Usage (0x55)
    0x95, 0x3F,    	//   Report Count (63)
    0xB1, 0x02,    	//   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,          	// End Collection

    // 507 bytes


};

__code uint8_t PS4DeviceDescriptor[] = {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x00, 0x02,  // bcdUSB 2.00
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x40,        // bMaxPacketSize0 64
    0x4C, 0x05,  // idVendor 0x054C
    0xCC, 0x09,  // idProduct 0x09CC
    0x00, 0x01,  // bcdDevice 2.00
    0x01,        // iManufacturer (String Index)
    0x02,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

    // 18 bytes

};

__code uint8_t PS4ConfigDescriptor[] = {
    0x09,        // bLength
    0x02,        // bDescriptorType (Configuration)
    0xE1, 0x00,  // wTotalLength 225
    0x04,        // bNumInterfaces 4
    0x01,        // bConfigurationValue
    0x00,        // iConfiguration (String Index)
    0xC0,        // bmAttributes Self Powered
    0xFA,        // bMaxPower 500mA

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x00,        // bInterfaceNumber 0
    0x00,        // bAlternateSetting
    0x00,        // bNumEndpoints 0
    0x01,        // bInterfaceClass (Audio)
    0x01,        // bInterfaceSubClass (Audio Control)
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x0A,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x01,        // bDescriptorSubtype (CS_INTERFACE -> HEADER)
    0x00, 0x01,  // bcdADC 1.00
    0x47, 0x00,  // wTotalLength 71
    0x02,        // binCollection 0x02
    0x01,        // baInterfaceNr 1
    0x02,        // baInterfaceNr 2

    0x0C,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x02,        // bDescriptorSubtype (CS_INTERFACE -> INPUT_TERMINAL)
    0x01,        // bTerminalID
    0x01, 0x01,  // wTerminalType (USB Streaming)
    0x06,        // bAssocTerminal
    0x02,        // bNrChannels 2
    0x03, 0x00,  // wChannelConfig (Left and Right Front)
    0x00,        // iChannelNames
    0x00,        // iTerminal

    0x0A,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x06,        // bDescriptorSubtype (CS_INTERFACE -> FEATURE_UNIT)
    0x02,        // bUnitID
    0x01,        // bSourceID
    0x01,        // bControlSize 1
    0x03, 0x00,  // bmaControls[0] (Mute,Volume)
    0x00, 0x00,  // bmaControls[1] (None)

    0x09,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x03,        // bDescriptorSubtype (CS_INTERFACE -> OUTPUT_TERMINAL)
    0x03,        // bTerminalID
    0x02, 0x04,  // wTerminalType (Headset)
    0x04,        // bAssocTerminal
    0x02,        // bSourceID
    0x00,        // iTerminal

    0x0C,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x02,        // bDescriptorSubtype (CS_INTERFACE -> INPUT_TERMINAL)
    0x04,        // bTerminalID
    0x02, 0x04,  // wTerminalType (Headset)
    0x03,        // bAssocTerminal
    0x01,        // bNrChannels 1
    0x00, 0x00,  // wChannelConfig
    0x00,        // iChannelNames
    0x00,        // iTerminal

    0x09,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x06,        // bDescriptorSubtype (CS_INTERFACE -> FEATURE_UNIT)
    0x05,        // bUnitID
    0x04,        // bSourceID
    0x01,        // bControlSize 1
    0x03, 0x00,  // bmaControls[0] (Mute,Volume)
    0x00,        // iFeature

    0x09,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x03,        // bDescriptorSubtype (CS_INTERFACE -> OUTPUT_TERMINAL)
    0x06,        // bTerminalID
    0x01, 0x01,  // wTerminalType (USB Streaming)
    0x01,        // bAssocTerminal
    0x05,        // bSourceID
    0x00,        // iTerminal

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x01,        // bInterfaceNumber 1
    0x00,        // bAlternateSetting
    0x00,        // bNumEndpoints 0
    0x01,        // bInterfaceClass (Audio)
    0x02,        // bInterfaceSubClass (Audio Streaming)
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x01,        // bInterfaceNumber 1
    0x01,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x01,        // bInterfaceClass (Audio)
    0x02,        // bInterfaceSubClass (Audio Streaming)
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x07,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x01,        // bDescriptorSubtype (CS_INTERFACE -> AS_GENERAL)
    0x01,        // bTerminalLink
    0x01,        // bDelay 1
    0x01, 0x00,  // wFormatTag (PCM)

    0x0B,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x02,        // bDescriptorSubtype (CS_INTERFACE -> FORMAT_TYPE)
    0x01,        // bFormatType 1
    0x02,        // bNrChannels (Stereo)
    0x02,        // bSubFrameSize 2
    0x10,        // bBitResolution 16
    0x01,        // bSamFreqType 1
    0x00, 0x7D, 0x00,  // tSamFreq[1] 32000 Hz

    0x09,        // bLength
    0x05,        // bDescriptorType (See Next Line)
    0x01,        // bEndpointAddress (OUT/H2D)
    0x09,        // bmAttributes (Isochronous, Adaptive, Data EP)
    0x84, 0x00,  // wMaxPacketSize 132
    0x01,        // bInterval 1 (unit depends on device speed)
    0x00,        // bRefresh
    0x00,        // bSyncAddress

    0x07,        // bLength
    0x25,        // bDescriptorType (See Next Line)
    0x01,        // bDescriptorSubtype (CS_ENDPOINT -> EP_GENERAL)
    0x00,        // bmAttributes (None)
    0x00,        // bLockDelayUnits
    0x00, 0x00,  // wLockDelay 0

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x02,        // bInterfaceNumber 2
    0x00,        // bAlternateSetting
    0x00,        // bNumEndpoints 0
    0x01,        // bInterfaceClass (Audio)
    0x02,        // bInterfaceSubClass (Audio Streaming)
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x02,        // bInterfaceNumber 2
    0x01,        // bAlternateSetting
    0x01,        // bNumEndpoints 1
    0x01,        // bInterfaceClass (Audio)
    0x02,        // bInterfaceSubClass (Audio Streaming)
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x07,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x01,        // bDescriptorSubtype (CS_INTERFACE -> AS_GENERAL)
    0x06,        // bTerminalLink
    0x01,        // bDelay 1
    0x01, 0x00,  // wFormatTag (PCM)

    0x0B,        // bLength
    0x24,        // bDescriptorType (See Next Line)
    0x02,        // bDescriptorSubtype (CS_INTERFACE -> FORMAT_TYPE)
    0x01,        // bFormatType 1
    0x01,        // bNrChannels (Mono)
    0x02,        // bSubFrameSize 2
    0x10,        // bBitResolution 16
    0x01,        // bSamFreqType 1
    0x80, 0x3E, 0x00,  // tSamFreq[1] 16000 Hz

    0x09,        // bLength
    0x05,        // bDescriptorType (See Next Line)
    0x82,        // bEndpointAddress (IN/D2H)
    0x05,        // bmAttributes (Isochronous, Async, Data EP)
    0x22, 0x00,  // wMaxPacketSize 34
    0x01,        // bInterval 1 (unit depends on device speed)
    0x00,        // bRefresh
    0x00,        // bSyncAddress

    0x07,        // bLength
    0x25,        // bDescriptorType (See Next Line)
    0x01,        // bDescriptorSubtype (CS_ENDPOINT -> EP_GENERAL)
    0x00,        // bmAttributes (None)
    0x00,        // bLockDelayUnits
    0x00, 0x00,  // wLockDelay 0

    0x09,        // bLength
    0x04,        // bDescriptorType (Interface)
    0x03,        // bInterfaceNumber 3
    0x00,        // bAlternateSetting
    0x02,        // bNumEndpoints 2
    0x03,        // bInterfaceClass
    0x00,        // bInterfaceSubClass
    0x00,        // bInterfaceProtocol
    0x00,        // iInterface (String Index)

    0x09,        // bLength
    0x21,        // bDescriptorType (HID)
    0x11, 0x01,  // bcdHID 1.11
    0x00,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0xFB, 0x01,  // wDescriptorLength[0] 507

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x84,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x05,        // bInterval 5 (unit depends on device speed)

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x03,        // bEndpointAddress (OUT/H2D)
    0x03,        // bmAttributes (Interrupt)
    0x40, 0x00,  // wMaxPacketSize 64
    0x05,        // bInterval 5 (unit depends on device speed)

    // 225 bytes
};



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


__code uint8_t FakeG304DeviceDescriptor []= {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x10, 0x01,  // bcdUSB 1.10
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x08,        // bMaxPacketSize0 8
    0xA7, 0x1E,  // idVendor 0x1EA7
    0x64, 0x00,  // idProduct 0x64
    0x00, 0x02,  // bcdDevice 4.00
    0x00,        // iManufacturer (String Index)
    0x01,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

// 18 bytes

};

__code uint8_t FakeG304ConfigDescriptor[] = {
    0x09,        // bLength
    0x02,        // bDescriptorType (Configuration)
    0x22, 0x00,  // wTotalLength 34
    0x01,        // bNumInterfaces 1
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
    0x10, 0x01,  // bcdHID 1.10
    0x00,        // bCountryCode
    0x01,        // bNumDescriptors
    0x22,        // bDescriptorType[0] (HID)
    0x69, 0x00,  // wDescriptorLength[0] 105

    0x07,        // bLength
    0x05,        // bDescriptorType (Endpoint)
    0x81,        // bEndpointAddress (IN/D2H)
    0x03,        // bmAttributes (Interrupt)
    0x08, 0x00,  // wMaxPacketSize 8
    0x02,        // bInterval 2 (unit depends on device speed)

// 34 bytes

};

__code uint8_t FakeG304ReportDescriptor[] = {
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

    // 105 bytes

};