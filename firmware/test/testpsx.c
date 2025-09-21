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


__xdata uint8_t PSXAdapterTestDataU[] =  { 0x01, 0x80, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDU[] = { 0x01, 0x00, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDD[] = { 0x01, 0xFF, 0x80, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDL[] = { 0x01, 0x80, 0x00, 0x64, 0x80, 0x0F, 0x00, 0x00 };
__xdata uint8_t PSXAdapterTestDataDR[] = { 0x01, 0x80, 0xFF, 0x64, 0x80, 0x0F, 0x00, 0x00 };

__xdata uint8_t PSXAdapterTestDataLeftClick[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x01, 0x00 };
__xdata uint8_t PSXAdapterTestDataRightClick[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x02, 0x00 };
__xdata uint8_t PSXAdapterTestDataBothClick[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x03, 0x00 };

void TestPSXAdapter(void) {

    InitPS2Ports();
    InitPresets();
    InitMice();

    InitTest(&UsbDev, PSXAdapterDeviceDescriptor, 18, PSXAdapterConfigDescriptor, 34);

    __xdata INTERFACE *pInterface = (__xdata INTERFACE *)ListGetData(UsbDev.Interfaces, 0);

    assert(!InterfaceParseReportDescriptor(pInterface, PSXAdapterReportDescriptor, 202));

    __xdata HID_REPORT *report = (__xdata HID_REPORT *)ListGetData(pInterface->Reports, 0);

    //DumpHID(pInterface);

    int c;

    uint8_t * chonk;

    /*assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataU));

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDU));
    HandleMouse();
    chonk = GetNextChonk();


    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDD));
    HandleMouse();
    chonk = GetNextChonk();
    //printf("ch %X - %X\n", chonk[2], chonk[3]);

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDL));
    HandleMouse();
    chonk = GetNextChonk();
    //printf("ch %X - %X\n", chonk[2], chonk[3]);

    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataDR));
    HandleMouse();
    chonk = GetNextChonk();
    //printf("ch %X - %X\n", chonk[2], chonk[3]);*/

    // mouse mode left click
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataLeftClick));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    printf("ch %x\n", chonk[1]);
    assert ((chonk[1] & 0x07) == 0x01);

    // release 
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataU));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    assert ((chonk[1] & 0x07) == 0x00);

    // mouse mode right click
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataRightClick));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    assert ((chonk[1] & 0x07) == 0x02);

    // release 
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataU));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    assert ((chonk[1] & 0x07) == 0x00);

    // mouse mode both click
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataBothClick));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    assert ((chonk[1] & 0x07) == 0x03);

    // release 
    assert (ParseReport(pInterface, 8 * 8, PSXAdapterTestDataU));
    HandleMouse();
    chonk = GetNextChonk();
    assert (chonk != NULL);
    assert ((chonk[1] & 0x07) == 0x00);

    printf("PSX Adapter Test Passed\n");

    //DumpHID(pInterface);
}