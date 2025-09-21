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


__xdata uint8_t MouseTestData[] = { 0, 0, 0 };


int main(){
    TestSetup();
    InitPS2Ports();
    InitMice();

    InitTest(&UsbDev, CheapoKeyboardDeviceDescriptor, 18, CheapoKeyboardConfigDescriptor, 59);
    
    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);
    assert(!InterfaceParseReportDescriptor(pInterface, StandardMouseDescriptor, 50));

    // put a bunch of stuff in the buffer (should be 60 entries)
    int c;
    uint8_t * chonk;

    // test buttons
    // no movement
    MouseTestData[1] = 0;
    MouseTestData[2] = 0;

    // 3 buttons gives us 8 combinations
    for (c = 1; c < 8; c++){
        // press
        MouseTestData[0] = c;
        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        assert (chonk != NULL);
        assert ((chonk[1] & 0x07) == c);

        // unpress
        MouseTestData[0] = 0;
        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        assert (chonk != NULL);
        assert ((chonk[1] & 0x07) == 0);
    }

    // random movement test
    for (c = 0; c < 255; c++){
        MouseTestData[0];
        MouseTestData[1] = (uint8_t)rand();
        MouseTestData[2] = (uint8_t)rand();

        // zero sized moves might not trigger a chonk
        if (MouseTestData[1] == 0) continue;
        if (MouseTestData[2] == 0) continue;

        assert (ParseReport(pInterface, 8 * 3, MouseTestData));
        HandleMouse();
        chonk = GetNextChonk();
        //printf("%X %X - %X %X %X %X\n", MouseTestData[1], MouseTestData[2], chonk[0], chonk[1], chonk[2], (uint8_t)(255-chonk[3])+(uint8_t)1);
        assert (chonk != NULL);
        assert (MouseTestData[1] == chonk[2]);
        assert (MouseTestData[2] == (uint8_t)(( 255 - chonk[3]) + 1));
    }

    printf("PASS\n");
    halt();
}