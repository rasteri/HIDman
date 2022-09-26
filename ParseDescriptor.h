#ifndef _PARSE_DESCRIPTOR_H_
#define _PARSE_DEXCRIPTOR_H_

extern BOOL ParseDeviceDescriptor(USB_DEV_DESCR *pDevDescr, UINT8 len, USB_DEVICE *pUsbDevice);

extern BOOL ParseConfigDescriptor(USB_CFG_DESCR *pCfgDescr, UINT16 len, USB_DEVICE *pUsbDevice);


#include "type.h"
#define REPORTPOOLSIZE 20
#define SEGMENTPOOLSIZE 100

extern __xdata HID_SEG SegmentPool[SEGMENTPOOLSIZE];
extern __xdata HID_REPORT ReportPool[REPORTPOOLSIZE];

extern __xdata uint8_t SegmentPoolSize;
extern __xdata uint8_t ReportPoolSize;

BOOL ParseReportDescriptor(uint8_t *pDescriptor, UINT16 len, HID_REPORT_DESC *pHidSegStruct, uint8_t port);
bool ParseReport(HID_REPORT_DESC *desc, uint32_t len, uint8_t *report);

#endif

