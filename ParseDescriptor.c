#include "Type.h"

#include "UsbDef.h"
#include "UsbHost2.h"


BOOL ParseDeviceDescriptor(const USB_DEV_DESCR *const pDevDescr, UINT8 len, USB_DEVICE *const pUsbDevice)
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

BOOL ParseConfigDescriptor(const USB_CFG_DESCR *const pCfgDescr, UINT16 len, USB_DEVICE *const pUsbDevice)
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

#define LITTLE_EADIAN       0
static UINT16  GetUnaligned16(const UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT16 *)start;
#else
	UINT16 dat = (*start) | (*(start + 1) << 8);
	
	return dat;
#endif
}

static UINT32  GetUnaligned32(const UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT32 *)start;
#else
	UINT32 dat = (*start) | (*(start + 1) << 8) | (*(start + 2) << 16) | (*(start + 3) << 24);
	
	return dat;
#endif
}

static UINT32  ItemUData(const HID_ITEM* itemInfo)
{
	switch (itemInfo->size)
	{
	case 1:  return itemInfo->value.u8;
	case 2:  return itemInfo->value.u16;
	case 4:  return itemInfo->value.u32;
	}

	return 0;
}

static INT32  ItemSData(const HID_ITEM* itemInfo)
{
	switch (itemInfo->size)
	{
	case 1:  return itemInfo->value.s8;
	case 2:  return itemInfo->value.s16;
	case 4:  return itemInfo->value.s32;
	}
	
	return 0;
}

static void InitHidGlobal(HID_GLOBAL *pHidGlobal)
{
	pHidGlobal->usagePage = USAGE_PAGE_NONE;
	pHidGlobal->logicalMinimum = 0;
	pHidGlobal->logicalMaximum = 0;
	pHidGlobal->physicalMinimum = 0;
	pHidGlobal->physicalMaximum = 0;
	pHidGlobal->unitExponent = 0;
	pHidGlobal->unit = 0;
	pHidGlobal->reportID = 0;
	pHidGlobal->reportSize = 0;
	pHidGlobal->reportCount = 0;
}

static const UINT8 *FetchItem(const UINT8 *start, const UINT8 *end, HID_ITEM *item)
{
	UINT8 b;
	if (end - start <= 0)
	{
		return NULL;
	}

	b = *start++;

	item->type = (b >> 2) & 3;
	item->tag  = (b >> 4) & 15;

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
		start = (UINT8 *)((UINT16 *)start + 1);

		return start;

	case 3:
		item->size++;
		if (end - start < 4)
		{
			return NULL;
		}

		item->value.u32 = GetUnaligned32(start);
		start = (UINT8 *)((UINT32 *)start + 1);
		return start;
	}

	return NULL;
}

static void InitHidSeg(HID_SEG_STRUCT *pHidSegStruct)
{
	UINT8 i;
	for (i = 0; i < HID_SEG_NUM; i++)
	{
		pHidSegStruct->HIDSeg[i].start = 0xff;
		pHidSegStruct->HIDSeg[i].size = 0;
		pHidSegStruct->HIDSeg[i].count = 0;
	}

	pHidSegStruct->KeyboardReportId = 0x00;
	pHidSegStruct->MouseReportId = 0x00;
}

BOOL ParseReportDescriptor(UINT8 *pDescriptor, UINT16 len, HID_SEG_STRUCT *pHidSegStruct)
{
	UINT32 startBit = 0;
	
	static __xdata HID_GLOBAL	hidGlobal;
	static __xdata USAGE arrUsage[MAX_USAGE_NUM];
	UINT8 usagePtr = 0;
	
	UINT8 *start = pDescriptor;
	UINT8 *end;
	HID_ITEM item;

	InitHidGlobal(&hidGlobal);
	InitHidSeg(pHidSegStruct);

	end = start + len;
	
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
				//是输入数据
				if (hidGlobal.usagePage == USAGE_PAGE_KEYBOARD)
				{
					//是按键

					pHidSegStruct->KeyboardReportId = hidGlobal.reportID;
					
					if (pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].size == 0)
					{
						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].start = startBit;

						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].size = hidGlobal.reportSize;
						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].count = hidGlobal.reportCount;
					}
					else
					{
						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].start = startBit;

						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].size = hidGlobal.reportSize;
						pHidSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].count = hidGlobal.reportCount;
					}
					
					startBit += hidGlobal.reportSize * hidGlobal.reportCount;
				}
				else if (hidGlobal.usagePage == USAGE_PAGE_BUTTON)
				{
					//是按键
					UINT32 dat = ItemUData(&item);

					pHidSegStruct->MouseReportId = hidGlobal.reportID;
					
					if (!(dat & 0x01))
					{
						//为变量
						pHidSegStruct->HIDSeg[HID_SEG_BUTTON_INDEX].start = startBit;
						pHidSegStruct->HIDSeg[HID_SEG_BUTTON_INDEX].size = hidGlobal.reportSize;
						pHidSegStruct->HIDSeg[HID_SEG_BUTTON_INDEX].count = hidGlobal.reportCount;
					}
					
					startBit += hidGlobal.reportSize * hidGlobal.reportCount;
				}
				else
				{
					if (usagePtr > 0)
					{
						//有usage
						UINT8 i = 0;
						while (i < usagePtr)
						{
							if (arrUsage[i].usageLen == 1)
							{
								if (arrUsage[i].usage == USAGE_X)
								{
									//是x
									pHidSegStruct->HIDSeg[HID_SEG_X_INDEX].start = startBit;
									pHidSegStruct->HIDSeg[HID_SEG_X_INDEX].size = hidGlobal.reportSize;
									pHidSegStruct->HIDSeg[HID_SEG_X_INDEX].count = 1;

									startBit += hidGlobal.reportSize;
								}
								else if (arrUsage[i].usage == USAGE_Y)
								{
									//是y
									pHidSegStruct->HIDSeg[HID_SEG_Y_INDEX].start = startBit;
									pHidSegStruct->HIDSeg[HID_SEG_Y_INDEX].size = hidGlobal.reportSize;
									pHidSegStruct->HIDSeg[HID_SEG_Y_INDEX].count = 1;

									startBit += hidGlobal.reportSize;
								}
								else if (arrUsage[i].usage == USAGE_WHEEL)
								{
									//是滚轮
									pHidSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].start = startBit;
									pHidSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].size = hidGlobal.reportSize;
									pHidSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].count = 1;

									startBit += hidGlobal.reportSize;
								}
								else
								{
									//其它page
									if (usagePtr == 1)
									{
										//只有一个page
										startBit += hidGlobal.reportSize * hidGlobal.reportCount;
									}
									else
									{
										//有多个page
										startBit += hidGlobal.reportSize;
									}
								}
							}
							else
							{
								if (usagePtr == 1)
								{
									//只有一个page
									startBit += hidGlobal.reportSize * hidGlobal.reportCount;
								}
								else
								{
									//有多个page
									startBit += hidGlobal.reportSize;
								}
							}
							
							i++;
						}
					}
				}	
			}
			
			usagePtr = 0;
			
			break;
			
		case TYPE_GLOBAL:

			switch (item.tag)
			{
			case HID_GLOBAL_ITEM_TAG_REPORT_ID:
				//report id
				startBit += item.size * 8;

				hidGlobal.reportID = ItemUData(&item);

				break;
			
			case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:				
				hidGlobal.logicalMinimum = ItemSData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
				if (hidGlobal.logicalMinimum < 0)
				{
					hidGlobal.logicalMaximum = ItemSData(&item);
				}
				else
				{
					hidGlobal.logicalMaximum = ItemUData(&item);
				}
				
				break;

			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
				hidGlobal.physicalMinimum = ItemSData(&item);

				break;
				
			case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
				if (hidGlobal.physicalMinimum < 0)
				{
					hidGlobal.physicalMaximum = ItemSData(&item);
				}
				else
				{
					hidGlobal.physicalMaximum = ItemUData(&item);
				}

				break;
				
			case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
				hidGlobal.reportSize = ItemUData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
				hidGlobal.reportCount = ItemUData(&item);

				break;

			case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
				hidGlobal.usagePage = ItemUData(&item);

				break;

			default:
				break;
			}
			
			break;

		case TYPE_LOCAL:
			if (item.tag == HID_LOCAL_ITEM_TAG_USAGE)
			{
				if (usagePtr < MAX_USAGE_NUM)
				{
					arrUsage[usagePtr].usage = ItemUData(&item);
					arrUsage[usagePtr].usageLen = item.size;

					usagePtr++;
				}
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

