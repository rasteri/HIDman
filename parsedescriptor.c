#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "usbdef.h"
#include "ch559.h"

#include "defs.h"

#include "usbhost.h"
#include "ps2protocol.h"
#include "data.h"
#include "ps2.h"
#include "util.h"
#include "parsedescriptor.h"
#include "andyalloc.h"

uint8_t JoyNum = 0;

BOOL ParseDeviceDescriptor(USB_DEV_DESCR *pDevDescr, UINT8 len, USB_DEVICE *pUsbDevice)
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

BOOL ParseConfigDescriptor(USB_CFG_DESCR *pCfgDescr, UINT16 len, USB_DEVICE *pUsbDevice)
{
	int index;

	UINT8 *pDescr;
	DESCR_HEADER *pDescrHeader;
	USB_ITF_DESCR *pItfDescr;
	USB_HID_DESCR *pHidDescr;
	USB_ENDP_DESCR *pEdpDescr;

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

	pUsbDevice->Interface = AllocInterface(pUsbDevice->InterfaceNum);

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

			if (pItfDescr->bAlternateSetting == 0)
			{
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].EndpointNum = 0;

				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].InterfaceClass = pItfDescr->bInterfaceClass;
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].InterfaceProtocol = pItfDescr->bInterfaceProtocol;
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].InterfaceSubClass = pItfDescr->bInterfaceSubClass;
			}
		}
		else if (descrType == USB_DESCR_TYP_HID)
		{
			//HID descriptor
			pHidDescr = (USB_HID_DESCR *)pDescr;
			if (pHidDescr->bDescriptorTypeX == USB_DESCR_TYP_REPORT)
			{
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].ReportSize = pHidDescr->wDescriptorLengthL | (pHidDescr->wDescriptorLengthH << 8);
			}
		}
		else if (descrType == USB_DESCR_TYP_ENDP)
		{
			//endpoint descriptor
			pEdpDescr = (USB_ENDP_DESCR *)pDescr;

			if (pUsbDevice->Interface[pItfDescr->bInterfaceNumber].EndpointNum >= MAX_ENDPOINT_NUM)
			{
				goto ONE_FINISH;
			}

			if (pItfDescr->bAlternateSetting == 0)
			{
				endpointIndex = pUsbDevice->Interface[pItfDescr->bInterfaceNumber].EndpointNum;
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].EndpointNum++;
				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].Endpoint[endpointIndex].EndpointAddr = pEdpDescr->bEndpointAddress & 0x7f;
				if (pEdpDescr->bEndpointAddress & 0x80)
				{
					pUsbDevice->Interface[pItfDescr->bInterfaceNumber].Endpoint[endpointIndex].EndpointDir = ENDPOINT_IN;
				}
				else
				{
					pUsbDevice->Interface[pItfDescr->bInterfaceNumber].Endpoint[endpointIndex].EndpointDir = ENDPOINT_OUT;
				}

				pUsbDevice->Interface[pItfDescr->bInterfaceNumber].Endpoint[endpointIndex].MaxPacketSize = pEdpDescr->wMaxPacketSizeL | (pEdpDescr->wMaxPacketSizeH << 8);
			}
		}
	ONE_FINISH:
		pDescr += pDescrHeader->bDescLength;

		index += pDescrHeader->bDescLength;
	}

	return TRUE;
}

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
	UINT32 dat = (*start) | (*(start + 1) << 8) | (*(start + 2) << 16) | (*(start + 3) << 24);

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
		return itemInfo->value.s8;
	case 2:
		return itemInfo->value.s16;
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

	return NULL;
}

#define CreateSeg()                                                                                              \
	{                                                                                                            \
		if (pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg == NULL)                                 \
		{                                                                                                        \
			pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg = (HID_SEG *)andyalloc(sizeof(HID_SEG)); \
			currSegPnt = pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg;                            \
		}                                                                                                        \
		else                                                                                                     \
		{                                                                                                        \
			currSegPnt->next = (HID_SEG *)andyalloc(sizeof(HID_SEG));                                            \
			currSegPnt = currSegPnt->next;                                                                       \
		}                                                                                                        \
		memset(currSegPnt, 0, sizeof(HID_SEG));                                                                  \
		currSegPnt->startBit = tempSB;                                                                           \
		currSegPnt->reportCount = hidGlobalPnt->reportCount;                                                     \
		tempSB += hidGlobalPnt->reportSize;                                                                      \
		currSegPnt->reportSize = hidGlobalPnt->reportSize;                                                       \
	}

//search though preset to see if this matches a mapping
#define CreateMapping()                                                        \
	{                                                                          \
		k = 0;                                                                 \
		DEBUG_OUT("rrrrs %hx\n", hidGlobalPnt->reportSize);                       \
		while (DefaultJoyMaps[k].InputType != MAP_TYPE_NONE)                   \
		{                                                                      \
			if (DefaultJoyMaps[k].InputUsagePage == hidGlobalPnt->usagePage && \
				DefaultJoyMaps[k].InputUsage == hidLocal.usage &&              \
				DefaultJoyMaps[k].Number == JoyNum)                            \
			{                                                                  \
				CreateSeg();                                                   \
				tempSB -= hidGlobalPnt->reportSize;                            \
				currSegPnt->OutputChannel = DefaultJoyMaps[k].OutputChannel;   \
				currSegPnt->OutputControl = DefaultJoyMaps[k].OutputControl;   \
				currSegPnt->InputType = DefaultJoyMaps[k].InputType;           \
				currSegPnt->InputParam = DefaultJoyMaps[k].InputParam;         \
			}                                                                  \
			k++;                                                               \
		}                                                                      \
		tempSB += hidGlobalPnt->reportSize;                                    \
	}

BOOL ParseReportDescriptor(uint8_t *pDescriptor, UINT16 len, HID_REPORT_DESC *pHidSegStruct, uint8_t port)
{
	static __xdata uint8_t i, k, collectionDepth, usagePtr, arrUsage[MAX_USAGE_NUM];
	static __xdata uint16_t startBit, tempSB, appUsage, appUsagePage;

	uint8_t *start, *end;

	static __xdata HID_GLOBAL hidGlobal;
	static __xdata HID_GLOBAL *hidGlobalPnt = &hidGlobal;
	static __xdata HID_LOCAL hidLocal;
	static __xdata HID_ITEM item;
	HID_SEG *currSegPnt;

	startBit = 0;
	tempSB = 0;
	appUsage = 0;
	appUsagePage = 0;
	i = 0;
	k = 0;
	collectionDepth = 0;
	usagePtr = 0;

	memset(pHidSegStruct, 0x00, sizeof(HID_REPORT_DESC));
	memset(hidGlobalPnt, 0x00, sizeof(HID_GLOBAL));
	memset(&hidLocal, 0x00, sizeof(HID_LOCAL));
	hidLocal.usageMax = 0xffff;
	hidLocal.usageMin = 0xffff;

	start = pDescriptor;
	end = start + len;

	JoyNum = 0;

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

				tempSB = startBit;
				if (pHidSegStruct->reports[hidGlobalPnt->reportID] == NULL)
				{
					pHidSegStruct->reports[hidGlobalPnt->reportID] = (HID_REPORT *)andyalloc(sizeof(HID_REPORT));
					memset(pHidSegStruct->reports[hidGlobalPnt->reportID], 0x00, sizeof(HID_REPORT));

					pHidSegStruct->reports[hidGlobalPnt->reportID]->appUsagePage = appUsagePage;
					pHidSegStruct->reports[hidGlobalPnt->reportID]->appUsage = appUsage;

					if (appUsagePage == REPORT_USAGE_PAGE_GENERIC && (appUsage == REPORT_USAGE_JOYSTICK || appUsage == REPORT_USAGE_GAMEPAD))
					{
						JoyNum++;
					}
				}

				if (ItemUData(&item) & HID_INPUT_VARIABLE)
				{

					// we found some discrete usages, get to it
					if (usagePtr)
					{
						// need to make a seg for each found usage
						for (i = 0; i < usagePtr; i++)
						{
							hidLocal.usage = arrUsage[i];

							if (appUsagePage == REPORT_USAGE_PAGE_GENERIC)
							{
								if (appUsage == REPORT_USAGE_MOUSE)
								{
									CreateSeg();
									if (hidGlobalPnt->usagePage == REPORT_USAGE_PAGE_GENERIC)
									{
										currSegPnt->OutputChannel = MAP_MOUSE;
										switch (arrUsage[i])
										{
										case REPORT_USAGE_X:
											// Mouse - value field
											currSegPnt->OutputControl = MAP_MOUSE_X;
											currSegPnt->InputType = MAP_TYPE_SCALE;
											break;

										case REPORT_USAGE_Y:
											// Mouse - value field
											currSegPnt->OutputControl = MAP_MOUSE_Y;
											currSegPnt->InputType = MAP_TYPE_SCALE;
											break;
										
										/*
										case REPORT_USAGE_WHEEL:
											// TODO: not working...
											// Mouse - value field
											currSegPnt->OutputControl = MAP_MOUSE_WHEEL;
											currSegPnt->InputType = MAP_TYPE_SCALE;
											break;
										*/
											
										}
									}
								}
								else if (appUsage == REPORT_USAGE_JOYSTICK || appUsage == REPORT_USAGE_GAMEPAD)
								{
									CreateMapping();
								}
							}
						}
					}
					// if no usages found, maybe a bitfield
					else if (hidLocal.usageMin != 0xFFFF && hidLocal.usageMax != 0xFFFF &&
							 hidGlobalPnt->reportSize == 1)
					{
						if (appUsagePage == REPORT_USAGE_PAGE_GENERIC)
						{
							if (appUsage == REPORT_USAGE_KEYBOARD)
							{
								if (hidGlobalPnt->usagePage == REPORT_USAGE_PAGE_KEYBOARD)
								{
									CreateSeg();
									// Keyboard - 1 bit per key (usually for modifier field)
									currSegPnt->OutputChannel = MAP_KEYBOARD;
									currSegPnt->OutputControl = hidLocal.usageMin;
									currSegPnt->InputType = MAP_TYPE_BITFIELD;
								}
							}
							else if (appUsage == REPORT_USAGE_MOUSE)
							{
								if (hidGlobalPnt->usagePage == REPORT_USAGE_PAGE_BUTTON)
								{
									CreateSeg();
									// Mouse - 1 bit per button
									currSegPnt->OutputChannel = MAP_MOUSE;
									currSegPnt->OutputControl = hidLocal.usageMin;
									currSegPnt->InputType = MAP_TYPE_BITFIELD;
								}
							}
							else if (appUsage == REPORT_USAGE_JOYSTICK || appUsage == REPORT_USAGE_GAMEPAD)
							{
								for (i = hidLocal.usageMin; i < hidLocal.usageMax; i++)
								{
									hidLocal.usage = i;
									CreateMapping();
								}
							}
						}
					}
					else
					{
					}
				}
				// Item is array style, whole range appears in every segment
				else
				{
					if (appUsagePage == REPORT_USAGE_PAGE_GENERIC &&
						appUsage == REPORT_USAGE_KEYBOARD &&
						hidGlobalPnt->usagePage == REPORT_USAGE_PAGE_KEYBOARD)
					{
						
						// need to make a seg for each report seg
						for (i = 0; i < hidGlobalPnt->reportCount; i++)
						{
					 		
							CreateSeg();
							currSegPnt->OutputChannel = MAP_KEYBOARD;
							currSegPnt->InputType = MAP_TYPE_ARRAY;
						}
					}
				}

				startBit += hidGlobalPnt->reportSize * hidGlobalPnt->reportCount;
				pHidSegStruct->reports[hidGlobalPnt->reportID]->length = startBit;
			}
			else if (item.tag == HID_MAIN_ITEM_TAG_COLLECTION_START)
			{
				collectionDepth++;
				if (ItemUData(&item) == HID_COLLECTION_APPLICATION)
				{
					// Make a note of this application collection's usage/page
					// (so we know what sort of device this is)
					appUsage = hidLocal.usage;

					appUsagePage = hidGlobalPnt->usagePage;
				}
			}
			else if (item.tag == HID_MAIN_ITEM_TAG_COLLECTION_END)
			{
				collectionDepth--;

				// Only advance app if we're at the root level
				if (collectionDepth == 0)
				{
					appUsage = 0x00;
					appUsagePage = 0x00;
				}
			}

			usagePtr = 0;
			hidLocal.usage = 0x00;
			hidLocal.usageMax = 0xffff;
			hidLocal.usageMin = 0xffff;

			break;

		case TYPE_GLOBAL:

			switch (item.tag)
			{
			case HID_GLOBAL_ITEM_TAG_REPORT_ID:
				pHidSegStruct->usesReports = 1;
				// report id
				startBit = 0;
				startBit += item.size * 8;
				//JoyNum++;

				hidGlobalPnt->reportID = ItemUData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
				hidGlobalPnt->logicalMinimum = ItemSData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
				if (hidGlobalPnt->logicalMinimum < 0)
				{
					hidGlobalPnt->logicalMaximum = ItemSData(&item);
				}
				else
				{
					hidGlobalPnt->logicalMaximum = ItemUData(&item);
				}

				break;

			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
				hidGlobalPnt->physicalMinimum = ItemSData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
				if (hidGlobalPnt->physicalMinimum < 0)
				{
					hidGlobalPnt->physicalMaximum = ItemSData(&item);
				}
				else
				{
					hidGlobalPnt->physicalMaximum = ItemUData(&item);
				}

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
				hidLocal.usage = ItemUData(&item);

				if (usagePtr < MAX_USAGE_NUM)
				{
					arrUsage[usagePtr] = ItemUData(&item);

					usagePtr++;
				}
			}
			else if (item.tag == HID_LOCAL_ITEM_TAG_USAGE_MIN)
			{
				hidLocal.usageMin = ItemUData(&item);
			}
			else if (item.tag == HID_LOCAL_ITEM_TAG_USAGE_MAX)
			{
				hidLocal.usageMax = ItemUData(&item);
			}

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