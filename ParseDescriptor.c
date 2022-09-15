#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ch559.h"
#include "Type.h"
#include "UsbDef.h"
#include "UsbHost.h"
#include "protocol.h"

#include "ps2.h"

#define LITTLE_EADIAN 1
static UINT16 GetUnaligned16(const UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT16 *)start;
#else
	UINT16 dat = (*start) | (*(start + 1) << 8);

	return dat;
#endif
}

static UINT32 GetUnaligned32(const UINT8 *start)
{
#if LITTLE_EADIAN
	return *(UINT32 *)start;
#else
	UINT32 dat = (UINT32)(*start) | ((UINT32)(*(start + 1)) << 8) | ((UINT32)(*(start + 2)) << 16) | ((UINT32)(*(start + 3)) << 24);

	return dat;
#endif
}

static UINT32 ItemUData(const HID_ITEM *itemInfo)
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

static INT32 ItemSData(const HID_ITEM *itemInfo)
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

static UINT8 *FetchItem(UINT8 *start, UINT8 *end, HID_ITEM *item)
{
	UINT8 b;
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

#define CreateNextSeg()                                                                                   \
	if (pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg == NULL)                              \
	{                                                                                                     \
		pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg = (HID_SEG *)malloc(sizeof(HID_SEG)); \
		currSegPnt = pHidSegStruct->reports[hidGlobalPnt->reportID]->firstHidSeg;                         \
	}                                                                                                     \
	else                                                                                                  \
	{                                                                                                     \
		currSegPnt->next = (HID_SEG *)malloc(sizeof(HID_SEG));                                            \
		currSegPnt = currSegPnt->next;                                                                    \
	}

BOOL ParseReportDescriptor(UINT8 *pDescriptor, UINT16 len, HID_REPORT_DESC *pHidSegStruct)
{
	uint32_t startBit = 0;
	uint8_t tmp = 0;
	static __xdata HID_GLOBAL *hidGlobalPnt;
	static __xdata HID_GLOBAL *tmpGlobal;
	static __xdata USAGE arrUsage[MAX_USAGE_NUM];
	static __xdata HID_LOCAL hidLocal;
	uint8_t collectionDepth = 0;
	UINT8 usagePtr = 0;
	uint32_t tempSB = 0;

	UINT8 *start = pDescriptor;
	UINT8 *end;
	static __xdata HID_ITEM item;

	uint32_t appUsage = 0;
	uint32_t appUsagePage = 0;

	HID_SEG *currSegPnt;

	memset(pHidSegStruct, 0x00, sizeof(pHidSegStruct));

	hidGlobalPnt = malloc(sizeof(HID_GLOBAL));
	memset(hidGlobalPnt, 0x00, sizeof(HID_GLOBAL));

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
				tempSB = startBit;
				if (pHidSegStruct->reports[hidGlobalPnt->reportID] == NULL)
				{
					pHidSegStruct->reports[hidGlobalPnt->reportID] = (HID_REPORT *)malloc(sizeof(HID_REPORT));
					pHidSegStruct->reports[hidGlobalPnt->reportID]->appUsagePage = appUsagePage;
					pHidSegStruct->reports[hidGlobalPnt->reportID]->appUsage = appUsage;
				}

				if (ItemUData(&item) & HID_INPUT_VARIABLE)
				{

					// we found some discrete usages, get to it
					if (usagePtr)
					{
						// need to make a seg for each found usage
						for (int i = 0; i < usagePtr; i++)
						{
							CreateNextSeg();
							printf("s1 %hx\n", (uint16_t)currSegPnt);
							currSegPnt->global = hidGlobalPnt;
							currSegPnt->startBit = tempSB;
							currSegPnt->usage = arrUsage[i].usage;
							currSegPnt->inputField = ItemUData(&item);
							tempSB += hidGlobalPnt->reportSize;
						}
					}
					// if no usages found, add min/max style
					else if (hidLocal.usageMin != 0xffff && hidLocal.usageMax != 0xFFFF)
					{
						// need to make a seg for each usage in the range
						for (uint32_t i = hidLocal.usageMin; i <= hidLocal.usageMax; i++)
						{
							CreateNextSeg();
							printf("s2 %hx\n", (uint16_t)currSegPnt);
							currSegPnt->global = hidGlobalPnt;
							currSegPnt->usage = i;
							currSegPnt->inputField = ItemUData(&item);
							currSegPnt->startBit = tempSB;
							tempSB += hidGlobalPnt->reportSize;
						}
					}
					else
					{
					}
				}
				// Item is array style, whole range appears in every segment
				else
				{
					// need to make a seg for each report seg
					for (int i = 0; i < hidGlobalPnt->reportCount; i++)
					{
						CreateNextSeg();
						printf("s3 %hx\n", (uint16_t)currSegPnt);
						currSegPnt->global = hidGlobalPnt;
						currSegPnt->inputField = ItemUData(&item);
						currSegPnt->usageMin = hidLocal.usageMin;
						currSegPnt->usageMax = hidLocal.usageMax;
						currSegPnt->startBit = tempSB;
						tempSB += hidGlobalPnt->reportSize;
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

			tmpGlobal = malloc(sizeof(HID_GLOBAL));
			memcpy(tmpGlobal, hidGlobalPnt, sizeof(HID_GLOBAL));
			hidGlobalPnt = tmpGlobal;

			switch (item.tag)
			{
			case HID_GLOBAL_ITEM_TAG_REPORT_ID:
				pHidSegStruct->usesReports = 1;
				// report id
				startBit = 0;
				startBit += item.size * 8;

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
					arrUsage[usagePtr].usage = ItemUData(&item);

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


