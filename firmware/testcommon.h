#pragma once
void UART_Init(void);
bool InterfaceParseReportDescriptor (__xdata INTERFACE * pInterface, uint8_t *Report, uint16_t ReportLen);
bool InterfaceParseDeviceDescriptors (
    USB_HUB_PORT * pUsbDevice,
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen);

void InitTest (
    USB_HUB_PORT *pUsbDevice,
    uint8_t *Dev, uint16_t DevLen, 
    uint8_t *Config, uint16_t ConfigLen
);

__xdata HID_SEG * FindSegByStartBit(__xdata HID_REPORT * descReport, uint16_t StartBit);

