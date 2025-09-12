//__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0x21, 0x43, 0x65, 0x00 };

__xdata uint8_t FakeG304TestData1[] = { 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
__xdata uint8_t FakeG304TestData2[] = { 0x02, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x00  };
__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x01 , 0x80 , 0x04 , 0x00 , 0x00 };
//__xdata uint8_t FakeG304TestData3[] = { 0x02 , 0x00 , 0x7F , 0x30 , 0x00 , 0x00 , 0x00  };
//__xdata uint8_t FakeG304TestData3[] = { 0x02, 0x00, 0x02 , 0xB0 , 0xFF , 0x00 , 0x00 };


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