
__xdata uint8_t CheapoGamepadTestDataU[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD1[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x1F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD2[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x2F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD3[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x4F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataD4[] = { 0x01, 0x80, 0x80, 0x7F, 0x7F, 0x8F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDL[] = { 0x01, 0x80, 0x80, 0x00, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDR[] = { 0x01, 0x80, 0x80, 0xFF, 0x7F, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDU[] = { 0x01, 0x80, 0x80, 0x7F, 0x00, 0x0F, 0x00, 0x00 };
__xdata uint8_t CheapoGamepadTestDataDD[] = { 0x01, 0x80, 0x80, 0x7F, 0xFF, 0x0F, 0x00, 0x00 };

void TestCheapoGamepad(void) {

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

    printf("End-to-end Standard Gamepad Test Passed\n");

    //DumpHID(pInterface);
}