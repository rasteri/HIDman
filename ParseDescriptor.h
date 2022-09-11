#ifndef _PARSE_DESCRIPTOR_H_
#define _PARSE_DEXCRIPTOR_H_

#include "type.h"

BOOL ParseReportDescriptor(UINT8 *pDescriptor, UINT16 len, HID_SEG_STRUCT *pHidSegStruct);

#endif

