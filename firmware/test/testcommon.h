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

uint32_t rand32(void);

extern __xdata USB_HUB_PORT UsbDev;

void TestSetup(void);
uint8_t * GetNextChunk(void);
uint8_t * GetNextChonk(void);

void halt(void);

extern __code uint32_t bitMasks32[];

/* Debugging enabled -- verify assertions at run time. */
void __assert(const char *expression, const char *functionname, const char *filename, unsigned int linenumber);
#define assert(...) ((__VA_ARGS__) ? (void)0 : __assert(#__VA_ARGS__, __func__, __FILE__, __LINE__))

