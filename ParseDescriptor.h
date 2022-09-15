#ifndef _PARSE_DESCRIPTOR_H_
#define _PARSE_DEXCRIPTOR_H_

#include "type.h"

BOOL ParseReportDescriptor(UINT8 *pDescriptor, UINT16 len, HID_REPORT_DESC *pHidSegStruct);
bool ParseReport(HID_REPORT_DESC *desc, uint32_t len, uint8_t *report);
#endif

