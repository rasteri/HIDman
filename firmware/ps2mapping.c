/*
    ps2mapping.c

    Handles the creation of USB->PS2 mappings
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
#include "data.h"
#include "ps2.h"
#include "parsedescriptor.h"
#include "andyalloc.h"
#include "system.h"
#include "preset.h"

JoyPreset *currPreset;
HID_SEG *currSegPnt = 0;

// running startbit that increments as segments get matched
uint16_t tempSB = 0;

void CreateSeg(INTERFACE *pInterface)                                                                                       
{                                 
	static HID_REPORT * __xdata rep;
	rep = (HID_REPORT *)ListGetData(pInterface->Reports, HIDParseState.hidGlobal.reportID);
	  
	rep->HidSegments = ListAdd(rep->HidSegments, sizeof(HID_SEG), 0x69);
	currSegPnt = (HID_SEG *)(rep->HidSegments->data);
	currSegPnt->startBit = tempSB;                                                                     
	currSegPnt->reportCount = HIDParseState.hidGlobal.reportCount;                                     
	tempSB += HIDParseState.hidGlobal.reportSize;                                                      
	currSegPnt->reportSize = HIDParseState.hidGlobal.reportSize; 

	if (HIDParseState.hidGlobal.logicalMinimum < 0) {
		currSegPnt->InputParam |= INPUT_PARAM_SIGNED;
	}                                      
}

//search though preset to see if this matches a mapping
void CreateMapping(INTERFACE *pInterface)                                                        
{                                                                          
	currPreset = JoyPresets;		
										
	while (currPreset != NULL)												
	{       

		if (currPreset->InputUsagePage == HIDParseState.hidGlobal.usagePage && 
			currPreset->InputUsage == HIDParseState.hidLocal.usage &&              
			currPreset->Number == HIDParseState.JoyNum)                            
		{                 
			CreateSeg(pInterface);                                                   
			tempSB -= HIDParseState.hidGlobal.reportSize;                            
			currSegPnt->OutputChannel = currPreset->OutputChannel;   
			currSegPnt->OutputControl = currPreset->OutputControl;   
			currSegPnt->InputType = currPreset->InputType;           
			currSegPnt->InputParam = currPreset->InputParam;    
     
		}                                                                 
		currPreset = currPreset->next; 										
	}                         
	                                            
	tempSB += HIDParseState.hidGlobal.reportSize;                                    
}



void CreateBitfieldMapping(__xdata INTERFACE *pHidSegStruct) {

	static __xdata uint8_t i;

	tempSB = HIDParseState.startBit;

	if (HIDParseState.appUsagePage == REPORT_USAGE_PAGE_GENERIC)
	{
		if (HIDParseState.appUsage == REPORT_USAGE_KEYBOARD)
		{
			if (HIDParseState.hidGlobal.usagePage == REPORT_USAGE_PAGE_KEYBOARD)
			{
				CreateSeg(pHidSegStruct);
				// Keyboard - 1 bit per key (usually for modifier field)
				currSegPnt->OutputChannel = MAP_KEYBOARD;
				currSegPnt->OutputControl = HIDParseState.hidLocal.usageMin;
				currSegPnt->InputType = MAP_TYPE_BITFIELD;
			}
		}
		else if (HIDParseState.appUsage == REPORT_USAGE_MOUSE)
		{
			if (HIDParseState.hidGlobal.usagePage == REPORT_USAGE_PAGE_BUTTON)
			{
				CreateSeg(pHidSegStruct);
				// Mouse - 1 bit per button
				currSegPnt->OutputChannel = MAP_MOUSE;
				currSegPnt->OutputControl = HIDParseState.hidLocal.usageMin;
				currSegPnt->InputType = MAP_TYPE_BITFIELD;
			}
		}
		else if (HIDParseState.appUsage == REPORT_USAGE_JOYSTICK || HIDParseState.appUsage == REPORT_USAGE_GAMEPAD)
		{
			for (i = HIDParseState.hidLocal.usageMin; i < HIDParseState.hidLocal.usageMax; i++)
			{
				HIDParseState.hidLocal.usage = i;
				CreateMapping(pHidSegStruct);
			}
		}
	}
}


void CreateUsageMapping(__xdata INTERFACE *pHidSegStruct){
	
	static __xdata uint8_t i;

	tempSB = HIDParseState.startBit;

	// need to make a seg for each found usage
	for (i = 0; i < HIDParseState.usagePtr; i++)
	{
		if (HIDParseState.appUsagePage == REPORT_USAGE_PAGE_GENERIC)
		{
			if (HIDParseState.appUsage == REPORT_USAGE_MOUSE)
			{
				CreateSeg(pHidSegStruct);
				if (HIDParseState.hidGlobal.usagePage == REPORT_USAGE_PAGE_GENERIC)
				{
					currSegPnt->OutputChannel = MAP_MOUSE;
					switch (HIDParseState.arrUsage[i])
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
					
					case REPORT_USAGE_WHEEL:
						// Mouse - value field
						currSegPnt->OutputControl = MAP_MOUSE_WHEEL;
						currSegPnt->InputType = MAP_TYPE_SCALE;
						break;
						
					}
				}
			}
			else if (HIDParseState.appUsage == REPORT_USAGE_JOYSTICK || HIDParseState.appUsage == REPORT_USAGE_GAMEPAD)
			{
				HIDParseState.hidLocal.usage = HIDParseState.arrUsage[i];
				CreateMapping(pHidSegStruct);
			}
		}
	}


}

void CreateArrayMapping(__xdata INTERFACE *pHidSegStruct){

	static __xdata uint8_t i;

	tempSB = HIDParseState.startBit;

	if (
		(HIDParseState.appUsagePage == REPORT_USAGE_PAGE_GENERIC &&
		HIDParseState.appUsage == REPORT_USAGE_KEYBOARD &&
		HIDParseState.hidGlobal.usagePage == REPORT_USAGE_PAGE_KEYBOARD)
		||
		(HIDParseState.appUsagePage == REPORT_USAGE_PAGE_CONSUMER &&
		HIDParseState.appUsage == REPORT_USAGE_CONSUMER_CONTROL)
	)
	{
		
		// need to make a seg for each report seg
		for (i = 0; i < HIDParseState.hidGlobal.reportCount; i++)
		{
			CreateSeg(pHidSegStruct);
			currSegPnt->OutputChannel = MAP_KEYBOARD;
			currSegPnt->InputType = MAP_TYPE_ARRAY;
		}
	}
}