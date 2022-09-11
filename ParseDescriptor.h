#ifndef _PARSE_DESCRIPTOR_H_
#define _PARSE_DEXCRIPTOR_H_

#include "type.h"

BOOL ParseReportDescriptor(const UINT8 *pDescriptor, UINT16 len, HID_SEG_STRUCT *const pHidSegStruct);

#endif

