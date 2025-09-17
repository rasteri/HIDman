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




__code uint8_t CW917DeviceDescriptor[] = {
    0x12,        // bLength
    0x01,        // bDescriptorType (Device)
    0x10, 0x01,  // bcdUSB 1.10
    0x00,        // bDeviceClass (Use class information in the Interface Descriptors)
    0x00,        // bDeviceSubClass 
    0x00,        // bDeviceProtocol 
    0x08,        // bMaxPacketSize0 8
    0xFA, 0x30,  // idVendor 0x30FA
    0x00, 0x04,  // idProduct 0x0400
    0x00, 0x01,  // bcdDevice 2.00
    0x00,        // iManufacturer (String Index)
    0x01,        // iProduct (String Index)
    0x00,        // iSerialNumber (String Index)
    0x01,        // bNumConfigurations 1

    // 18 bytes


};

__code uint8_t CW917ConfigDescriptor[] = {
    0x09,        //   bLength
    0x02,        //   bDescriptorType (Configuration)
    0x22, 0x00,  //   wTotalLength 34
    0x01,        //   bNumInterfaces 1
    0x01,        //   bConfigurationValue
    0x00,        //   iConfiguration (String Index)
    0xA0,        //   bmAttributes Remote Wakeup
    0x32,        //   bMaxPower 100mA

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
    0x40, 0x00,  //   wDescriptorLength[0] 64

    0x07,        //   bLength
    0x05,        //   bDescriptorType (Endpoint)
    0x81,        //   bEndpointAddress (IN/D2H)
    0x03,        //   bmAttributes (Interrupt)
    0x05, 0x00,  //   wMaxPacketSize 5
    0x0A,        //   bInterval 10 (unit depends on device speed)

    // 34 bytes

};

__code uint8_t CW917ReportDescriptor[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
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
    0x16, 0x00, 0xF8,  //     Logical Minimum (-2048)
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
    0xC0,              //   End Collection
    0xC0,              // End Collection

    // 64 bytes

};

__xdata uint8_t MouseTestData[] = { 0x00, 0xFF, 0xFF, 0xFF, 0x00 };

int main() {

    uint8_t * chonk;

    TestSetup();

    InitPS2Ports();
    InitMice();

    InitTest(&UsbDev, CW917DeviceDescriptor, 18, CW917ConfigDescriptor, 34);

    assert(UsbDev.InterfaceNum == 1);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, CW917ReportDescriptor, 64));

    // random movement test
    for (int c = 0; c < 200; c++) {

        // generate 12 bits of randomness
        uint32_t xrand = rand32() & 0xFFF;
        uint32_t yrand = rand32() & 0xFFF;

        int32_t xrands = (int32_t) SIGNEX(xrand, 11);
        int32_t yrands = (int32_t) SIGNEX(yrand, 11);

        // pack it into report
        MouseTestData[1] = xrand & 0xFF;
        MouseTestData[2] = ((xrand & 0xF00) >> 8) | ((yrand & 0x00F) << 4);
        MouseTestData[3] = (yrand & 0xFF0) >> 4;


        // zero sized moves might not trigger a chonk
        if (MouseTestData[1] == 0) continue;
        if (MouseTestData[2] == 0) continue;
        if (MouseTestData[3] == 0) continue;


        // parse the random mouse movement
        assert (ParseReport(pInterface, 40, MouseTestData));
        

        //see if it comes out the other end with correct value
        bool xzeroed = 0, yzeroed = 0;
        uint8_t count = 0;

        // because ps2 is limited to +-255, the result will come in several chonks, we have to accumulate them
        while (xrands != 0 || yrands != 0) {

            HandleMouse();
            chonk = GetNextChonk();
            assert (chonk != NULL);

            uint32_t processedx = SIGNEX(chonk[2] | ((chonk[1] & 0x10) << 4), 8);
            uint32_t processedy = - SIGNEX(chonk[3] | ((chonk[1] & 0x20) << 3), 8);

            xrands -= processedx;
            yrands -= processedy;

            // give up after 32 goes
            count++;
            assert(count < 32);

        }
    }


    printf("PASS\n");
    halt();
}