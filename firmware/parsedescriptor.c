/*
	parsedescriptor.c

	Handles the parsing of the various USB descriptors
	The HID report descriptor being the tough one

*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "usbdef.h"
#include "ch559.h"

#include "defs.h"

#include "usbhost.h"
#include "ps2protocol.h"
#include "ps2mapping.h"
#include "data.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "andyalloc.h"
#include "system.h"
#include "preset.h"
#include "linkedlist.h"



#define LITTLE_EADIAN 0
static UINT16 GetUnaligned16(UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT16 *)start;
#else
	UINT16 dat = (*start) | (*(start + 1) << 8);

	return dat;
#endif
}

static UINT32 GetUnaligned32(UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT32 *)start;
#else
	UINT32 dat = (UINT32)(*start) | ((UINT32)(*(start + 1)) << 8) | ((UINT32)(*(start + 2)) << 16) | ((UINT32)(*(start + 3)) << 24);

	return dat;
#endif
}

static UINT32 ItemUData(HID_ITEM *itemInfo)
{
	switch (itemInfo->size)
	{
	case 1:
		return itemInfo->value.u8;
	case 2:
		return itemInfo->value.u16;
	case 4:
		return itemInfo->value.u32;
	}

	return 0;
}

static INT32 ItemSData(HID_ITEM *itemInfo)
{
	switch (itemInfo->size)
	{
	case 1:
		return SIGNEX(itemInfo->value.s8, 7); //itemInfo->value.s8;
	case 2:
		return SIGNEX(itemInfo->value.s16, 15);//itemInfo->value.s16;
	case 4:
		return itemInfo->value.s32; 
	}

	return 0;
}

static uint8_t *FetchItem(uint8_t *start, uint8_t *end, HID_ITEM *item)
{
	static __xdata uint8_t b;

	if (end - start <= 0)
	{
		return NULL;
	}
	b = *start++;

	item->type = (b >> 2) & 3;
	item->tag = (b >> 4) & 15;

	if (item->tag == HID_ITEM_TAG_LONG)
	{
		item->format = HID_ITEM_FORMAT_LONG;

		if (end - start < 2)
		{
			return NULL;
		}

		item->size = *start++;
		item->tag = *start++;

		if (end - start < item->size)
		{
			return NULL;
		}

		item->value.longdata = start;
		start += item->size;

		return start;
	}

	item->format = HID_ITEM_FORMAT_SHORT;
	item->size = b & 3;

	switch (item->size)
	{
	case 0:
		return start;

	case 1:
		if (end - start < 1)
		{
			return NULL;
		}

		item->value.u8 = *start++;

		return start;

	case 2:
		if (end - start < 2)
		{
			return NULL;
		}

		item->value.u16 = GetUnaligned16(start);
		start += 2;

		return start;

	case 3:
		item->size++;
		if (end - start < 4)
		{
			return NULL;
		}

		item->value.u32 = GetUnaligned32(start);
		start += 4;
		return start;
	}

}


__xdata ParseState HIDParseState;

uint8_t DumpHID(INTERFACE *pInterface)
{
    LinkedList *tmpsegNode;
    HID_SEG *tmpsegment;
    uint8_t count = 0;
    for (uint8_t x = 0; x < MAX_REPORTS; x++)
    {
        HID_REPORT *tr = (HID_REPORT *)ListGetData(pInterface->Reports, x);
        if (tr != NULL)
        {
            tmpsegNode = tr->HidSegments;

            printf("Report %x, usage %x, length %u: \n", x, tr->appUsage, tr->length);

            while (tmpsegNode != NULL)
            {
                tmpsegment = (HID_SEG *)(tmpsegNode->data);
                printf("startbit %u, it %hx, ip %x, chan %hx, cont %hx, size %hx, count %hx\n", tmpsegment->startBit, tmpsegment->InputType, tmpsegment->InputParam, tmpsegment->OutputChannel, tmpsegment->OutputControl, tmpsegment->reportSize, tmpsegment->reportCount);
                tmpsegNode = tmpsegNode->next;
                count++;
            }
        }
    }
    return count;
}

BOOL ParseReportDescriptor(uint8_t *pDescriptor, UINT16 len, INTERFACE *pInterface)
{
	static __xdata uint8_t i, k, collectionDepth;

	static uint8_t * __xdata start;
	static uint8_t * __xdata end;

	static __xdata HID_GLOBAL * __xdata hidGlobalPnt = &HIDParseState.hidGlobal;
	static __xdata HID_ITEM item;

	static HID_REPORT * __xdata currHidReport;
	currHidReport = NULL;
	HIDParseState.startBit = 0;
	HIDParseState.appUsage = 0;
	HIDParseState.appUsagePage = 0;
	i = 0;
	k = 0;
	collectionDepth = 0;

	memset(&HIDParseState, 0x00, sizeof(ParseState));
	HIDParseState.hidLocal.usageMax = 0xffff;
	HIDParseState.hidLocal.usageMin = 0xffff;

	start = pDescriptor;
	end = start + len;

	HIDParseState.JoyNum = 0;

	while ((start = FetchItem(start, end, &item)) != NULL)
	{
		if (item.format != HID_ITEM_FORMAT_SHORT)
		{
			goto ERR;
		}

		switch (item.type)
		{
		case TYPE_MAIN:
			if (item.tag == HID_MAIN_ITEM_TAG_INPUT)
			{
				if (
					HIDParseState.appUsagePage == REPORT_USAGE_PAGE_GENERIC &&
					(
						HIDParseState.appUsage == REPORT_USAGE_JOYSTICK || 
						HIDParseState.appUsage == REPORT_USAGE_GAMEPAD ||
						HIDParseState.appUsage == REPORT_USAGE_KEYBOARD ||
						HIDParseState.appUsage == REPORT_USAGE_MOUSE
						
					)
				
				){
					if (currHidReport == NULL)
					{
						pInterface->Reports = ListAdd(pInterface->Reports, sizeof(HID_REPORT), hidGlobalPnt->reportID);
						currHidReport = (HID_REPORT *)(pInterface->Reports->data);

						currHidReport->appUsagePage = HIDParseState.appUsagePage;
						currHidReport->appUsage = HIDParseState.appUsage;

						if ((HIDParseState.appUsage == REPORT_USAGE_JOYSTICK || HIDParseState.appUsage == REPORT_USAGE_GAMEPAD))
							HIDParseState.JoyNum++;
					}

					if (ItemUData(&item) & HID_INPUT_VARIABLE)
					{
						// we found some discrete usages, get to it
						if (HIDParseState.usagePtr)
						{
							CreateUsageMapping(pInterface);
						}
						// if no usages found, maybe a bitfield
						else if (HIDParseState.hidLocal.usageMin != 0xFFFF && HIDParseState.hidLocal.usageMax != 0xFFFF &&
								hidGlobalPnt->reportSize == 1)
						{
							CreateBitfieldMapping(pInterface);
						}
						else
						{
						}
					}
					// Item is array style, whole range appears in every segment
					else
					{
						CreateArrayMapping(pInterface);
					}
				}
				HIDParseState.startBit += (uint16_t)hidGlobalPnt->reportSize * (uint16_t)hidGlobalPnt->reportCount;

				if (currHidReport != NULL) currHidReport->length = HIDParseState.startBit;
			}
			else if (item.tag == HID_MAIN_ITEM_TAG_COLLECTION_START)
			{
				collectionDepth++;
				if (ItemUData(&item) == HID_COLLECTION_APPLICATION)
				{
					// Make a note of this application collection's usage/page
					// (so we know what sort of device this is)
					HIDParseState.appUsage = HIDParseState.hidLocal.usage;

					HIDParseState.appUsagePage = hidGlobalPnt->usagePage;
				}
			}
			else if (item.tag == HID_MAIN_ITEM_TAG_COLLECTION_END)
			{
				collectionDepth--;

				// Only advance app if we're at the root level
				if (collectionDepth == 0)
				{
					HIDParseState.appUsage = 0x00;
					HIDParseState.appUsagePage = 0x00;
				}
			}

			HIDParseState.usagePtr = 0;
			HIDParseState.hidLocal.usage = 0x00;
			HIDParseState.hidLocal.usageMax = 0xffff;
			HIDParseState.hidLocal.usageMin = 0xffff;
			break;

		case TYPE_GLOBAL:

			switch (item.tag)
			{
			case HID_GLOBAL_ITEM_TAG_REPORT_ID:
				pInterface->usesReports = 1;
				// report id
				HIDParseState.startBit = 0;
				HIDParseState.startBit += item.size * 8;
				//JoyNum++;
				hidGlobalPnt->reportID = ItemUData(&item);
				currHidReport = NULL;
				break;

			case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
				hidGlobalPnt->logicalMinimum = ItemSData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
				if (hidGlobalPnt->logicalMinimum < 0)
					hidGlobalPnt->logicalMaximum = ItemSData(&item);
				else
					hidGlobalPnt->logicalMaximum = ItemUData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
				hidGlobalPnt->physicalMinimum = ItemSData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
				if (hidGlobalPnt->physicalMinimum < 0)
					hidGlobalPnt->physicalMaximum = ItemSData(&item);
				else
					hidGlobalPnt->physicalMaximum = ItemUData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
				hidGlobalPnt->reportSize = ItemUData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
				hidGlobalPnt->reportCount = ItemUData(&item);
				break;

			case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
				hidGlobalPnt->usagePage = ItemUData(&item);
				break;

			default:
				break;
			}

			break;

		case TYPE_LOCAL:
			if (item.tag == HID_LOCAL_ITEM_TAG_USAGE)
			{
				HIDParseState.hidLocal.usage = ItemUData(&item);

				if (HIDParseState.usagePtr < MAX_USAGE_NUM)
				{
					HIDParseState.arrUsage[HIDParseState.usagePtr] = ItemUData(&item);
					HIDParseState.usagePtr++;
				}
			}
			else if (item.tag == HID_LOCAL_ITEM_TAG_USAGE_MIN)
				HIDParseState.hidLocal.usageMin = ItemUData(&item);
			else if (item.tag == HID_LOCAL_ITEM_TAG_USAGE_MAX)
				HIDParseState.hidLocal.usageMax = ItemUData(&item);

			break;
		}

		if (start == end)
		{
			return TRUE;
		}
		
	}

ERR:
	return FALSE;
}

BOOL ParseDeviceDescriptor(USB_DEV_DESCR *pDevDescr, UINT8 len, USB_HUB_PORT *pUsbDevice)
{
	if (len > sizeof(USB_DEV_DESCR))
	{
		return FALSE;
	}

	pUsbDevice->MaxPacketSize0 = pDevDescr->bMaxPacketSize0;
	pUsbDevice->DeviceClass = pDevDescr->bDeviceClass;

	if (len == sizeof(USB_DEV_DESCR))
	{
		pUsbDevice->VendorID = pDevDescr->idVendorL | (pDevDescr->idVendorH << 8);
		pUsbDevice->ProductID = pDevDescr->idProductL | (pDevDescr->idProductH << 8);
		pUsbDevice->bcdDevice = pDevDescr->bcdDeviceL | (pDevDescr->bcdDeviceH << 8);
	}

	return TRUE;
}

BOOL ParseConfigDescriptor(USB_CFG_DESCR *pCfgDescr, UINT16 len, USB_HUB_PORT *pUsbDevice)
{
	int index;

	static  UINT8 * __xdata pDescr;
	static DESCR_HEADER * __xdata pDescrHeader;
	static USB_ITF_DESCR * __xdata pItfDescr = NULL;
	static USB_HID_DESCR * __xdata pHidDescr;
	static USB_ENDP_DESCR * __xdata pEdpDescr;
	static INTERFACE * __xdata currInt;

	UINT8 descrType;
	int endpointIndex;

	UINT16 totalLen = pCfgDescr->wTotalLengthL | (pCfgDescr->wTotalLengthH << 8);
	if (totalLen > len)
	{
		totalLen = len;
	}

	pUsbDevice->InterfaceNum = pCfgDescr->bNumInterfaces;
	if (pUsbDevice->InterfaceNum > MAX_INTERFACE_NUM)
	{
		pUsbDevice->InterfaceNum = MAX_INTERFACE_NUM;
	}

	pDescr = (UINT8 *)pCfgDescr;

	pDescr += pCfgDescr->bLength;

	index = pCfgDescr->bLength;

	while (index < totalLen)
	{
		pDescrHeader = (DESCR_HEADER *)pDescr;
		descrType = pDescrHeader->bDescriptorType;

		if (descrType == USB_DESCR_TYP_INTERF)
		{
			//interface descriptor
			pItfDescr = (USB_ITF_DESCR *)pDescr;
			if (pItfDescr->bInterfaceNumber >= pUsbDevice->InterfaceNum)
			{
				break;
			}

			if (pItfDescr->bInterfaceClass == USB_DEV_CLASS_HID)
			{
			
				pUsbDevice->Interfaces = ListAdd(pUsbDevice->Interfaces, sizeof(INTERFACE), pItfDescr->bInterfaceNumber);
				currInt = (INTERFACE*)(pUsbDevice->Interfaces->data);
				InitInterface(currInt);

				if (pItfDescr->bAlternateSetting == 0)
				{
					currInt->EndpointNum = 0;

					currInt->InterfaceClass = pItfDescr->bInterfaceClass;
					currInt->InterfaceProtocol = pItfDescr->bInterfaceProtocol;
					currInt->InterfaceSubClass = pItfDescr->bInterfaceSubClass;
				}
			}
		}
		else if (descrType == USB_DESCR_TYP_HID)
		{
			//HID descriptor
			pHidDescr = (USB_HID_DESCR *)pDescr;
			if (pHidDescr->bDescriptorTypeX == USB_DESCR_TYP_REPORT)
			{
				currInt->ReportSize = pHidDescr->wDescriptorLengthL | (pHidDescr->wDescriptorLengthH << 8);
			}
		}
		else if (descrType == USB_DESCR_TYP_ENDP)
		{
			//endpoint descriptor
			pEdpDescr = (USB_ENDP_DESCR *)pDescr;

			if (currInt->EndpointNum >= MAX_ENDPOINT_NUM)
			{
				goto ONE_FINISH;
			}

			if (pItfDescr->bAlternateSetting == 0)
			{
				endpointIndex = currInt->EndpointNum;
				currInt->EndpointNum++;
				currInt->Endpoint[endpointIndex].EndpointAddr = pEdpDescr->bEndpointAddress & 0x7f;
				if (pEdpDescr->bEndpointAddress & 0x80)
				{
					currInt->Endpoint[endpointIndex].EndpointDir = ENDPOINT_IN;
				}
				else
				{
					currInt->Endpoint[endpointIndex].EndpointDir = ENDPOINT_OUT;
				}

				currInt->Endpoint[endpointIndex].MaxPacketSize = pEdpDescr->wMaxPacketSizeL | (pEdpDescr->wMaxPacketSizeH << 8);
			}
		}
	ONE_FINISH:
		pDescr += pDescrHeader->bDescLength;

		index += pDescrHeader->bDescLength;
	}

	return TRUE;
}