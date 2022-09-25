#include "Type.h"

#include "Protocol.h"
#include "UsbHost.h"
#include "ParseHidData.h"

#include "Trace.h"

static UINT8C lowest_bit_bitmap[] =
{
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

static BOOL NewUsbKeyboardParse(UINT8 *pUsb, UINT8 len, KEYBOARD_PARSE_STRUCT *pKeyboardParseStruct, 
									UINT8 *pOut, UINT8 maxOut)
{  
	UINT8 i;

	UINT8 offset = 0;

	if (maxOut < HID_KEYBOARD_VAL_LEN)
	{
		return FALSE;
	}
	
	if (len > MAX_HID_KEYBOARD_BIT_VAL_LEN)
	{
		len = MAX_HID_KEYBOARD_BIT_VAL_LEN;
	}
	
	for (i = 0; i < len; i++) 
	{
		UINT8 d = pUsb[i] ^ pKeyboardParseStruct->KeyboardBitVal[i];

		while (d)
		{
			UINT8 lowestBit = lowest_bit_bitmap[d];
			UINT8 mask = 1u << lowestBit;
			UINT8 key = offset + lowestBit;
			UINT8 j;
			
			if (pUsb[i] & mask)
			{
				// make key
				for (j = 0; j < HID_KEYBOARD_VAL_LEN; j++)
				{
					if (pKeyboardParseStruct->KeyboardVal[j] == 0x00)
					{
						pKeyboardParseStruct->KeyboardVal[j] = key;

						break;
					}
				}

				if (j >= HID_KEYBOARD_VAL_LEN)
				{
					return FALSE;
				}

				pKeyboardParseStruct->KeyboardBitVal[i] |= mask;
			}
			else
			{
				//break key
				for (j = 0; j < HID_KEYBOARD_VAL_LEN; j++)
				{
					if (pKeyboardParseStruct->KeyboardVal[j] == key)
					{
						UINT8 k;
						for (k = j + 1; k < HID_KEYBOARD_VAL_LEN && pKeyboardParseStruct->KeyboardVal[k] != 0x00; k++)
						{
							pKeyboardParseStruct->KeyboardVal[k - 1] = pKeyboardParseStruct->KeyboardVal[k];
						}

						pKeyboardParseStruct->KeyboardVal[k - 1] = 0x00;

						break;
					}
				}

				
				if (j >= HID_KEYBOARD_VAL_LEN)
				{
					return FALSE;
				}
				
				pKeyboardParseStruct->KeyboardBitVal[i] &= ~mask;
			}
			
			d &= ~mask;
		}

		offset += 8;
	}

	for (i = 0; i < HID_KEYBOARD_VAL_LEN; i++)
	{
		pOut[i] = pKeyboardParseStruct->KeyboardVal[i];
	}
	
	return TRUE;
}

BOOL UsbKeyboardParse(UINT8 *pUsb, UINT8 *pOut, HID_SEG_STRUCT *pKeyboardSegStruct, KEYBOARD_PARSE_STRUCT *pKeyboardParseStruct)
{
    UINT8 i;

    BOOL ret;
    
    if (pKeyboardSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].start == 0xff)
    {
        return FALSE;
    }

	if (pKeyboardSegStruct->KeyboardReportId != 0 && pKeyboardSegStruct->KeyboardReportId != pUsb[0])
	{
		return FALSE;
	}
	
	ret = TRUE;
	
    if (pOut != NULL)
    {
        UINT8 index = pKeyboardSegStruct->HIDSeg[HID_SEG_KEYBOARD_MODIFIER_INDEX].start / 8;
        pOut[0] = pUsb[index];
        pOut[1] = 0x00;

        index = pKeyboardSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].start / 8;
        if (pKeyboardSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].size > 1)
        {
			for (i = 0; i < 6; i++)
	        {
	            pOut[2 + i] = pUsb[index + i];
	        }
        }
        else
        {
			//bit
			if (!NewUsbKeyboardParse(&pUsb[index], pKeyboardSegStruct->HIDSeg[HID_SEG_KEYBOARD_VAL_INDEX].count / 8, 
					pKeyboardParseStruct, &pOut[2], 6))
			{
				ret = FALSE;
			}
        }
    }

    return ret;
}

static UINT8 GetMouseFieldData(UINT8 *pData, UINT8 start, UINT8 size)
{
	UINT8 index = start / 8;
	UINT8 mouseData = pData[index];
	UINT8 i;
		
	start -= index * 8;
	
	mouseData >>= start;
	
	if (size > 8 - start)
	{
		size = 8 - start;
	}
	
	for (i = size; i < 8; i++)
	{
		mouseData &= ~(0x01 << i);
	}
	
	return mouseData;
}

//--------------------------------------------------------------------------------------------------
// ��������: UsbMouseParse()
// ��������: ������ݽ���
//--------------------------------------------------------------------------------------------------
BOOL UsbMouseParse(UINT8 *pUsb, UINT8 *pOut, HID_SEG_STRUCT *pMouseSegStruct)
{
	UINT8 index;
	
	UINT16 datx, daty;

	UINT8 size;

	if (pMouseSegStruct->MouseReportId != 0 && pMouseSegStruct->MouseReportId != pUsb[0])
	{
		return FALSE;
	}
	
	//����buttonλ��
	index = pMouseSegStruct->HIDSeg[HID_SEG_BUTTON_INDEX].start;
	if (index == 0xff)
	{
		pOut[0] = 0;
	}
	else
	{
		index >>= 3;
		
		pOut[0] = pUsb[index] & 0x07;
	}
		
	//����x��yλ��
	index = pMouseSegStruct->HIDSeg[HID_SEG_X_INDEX].start;
	if (index == 0xff)
	{
		pOut[1] = 0;
		pOut[2] = 0;
	}
	else
	{
		index >>= 3;

	    size = pMouseSegStruct->HIDSeg[HID_SEG_X_INDEX].size;
		if (size == 16)
		{
			datx = pUsb[index] | (pUsb[index + 1] << 8);
			daty = pUsb[index + 2] | (pUsb[index + 3] << 8);
			
			pOut[1] = (UINT8)datx;
			pOut[2] = (UINT8)daty;
		}
		else if (size == 12)
		{
			datx = pUsb[index] | ((pUsb[index + 1] & 0x0f) << 8);
			daty = ((pUsb[index + 1] >> 4) & 0x0f) | (pUsb[index + 2] << 4);
		
			pOut[1] = (UINT8)datx;
			pOut[2] = (UINT8)daty;		
		}
		else
		{
		    //size == 8
			pOut[1] = pUsb[index];		
			pOut[2] = pUsb[index + 1];
		}
	}
	
	//����wheelλ��
	index = pMouseSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].start;
	if (index == 0xff)
	{
		pOut[3] = 0x00;
	}
	else
	{
		if (pMouseSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].size > 0)
		{
			pOut[3] = GetMouseFieldData(pUsb, pMouseSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].start, pMouseSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].size);
		
			if (pOut[3] & (0x01 << (pMouseSegStruct->HIDSeg[HID_SEG_WHEEL_INDEX].size - 1)))
			{
				pOut[3] = 0xff;
			}
		}
		else
		{
			pOut[3] = 0x00;
		}	
	}

	return TRUE;
}


