#ifndef _PARSE_DESCRIPTOR_H_
#define _PARSE_DEXCRIPTOR_H_

extern BOOL ParseDeviceDescriptor(USB_DEV_DESCR *pDevDescr, UINT8 len, USB_DEVICE *pUsbDevice);

extern BOOL ParseConfigDescriptor(USB_CFG_DESCR *pCfgDescr, UINT16 len, USB_DEVICE *pUsbDevice);


#include "type.h"
#define REPORTPOOLSIZE 5
#define SEGMENTPOOLSIZE 50

extern __xdata HID_SEG SegmentPool[2][SEGMENTPOOLSIZE];
extern __xdata HID_REPORT ReportPool[2][REPORTPOOLSIZE];

extern __xdata uint8_t SegmentPoolSizes[2];
extern __xdata uint8_t ReportPoolSizes[2];

BOOL ParseReportDescriptor(uint8_t *pDescriptor, UINT16 len, HID_REPORT_DESC *pHidSegStruct, uint8_t port);
bool ParseReport(HID_REPORT_DESC *desc, uint32_t len, uint8_t *report);

#endif
