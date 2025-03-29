#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ch559.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "menu.h"
#include "mouse.h"
#include "pwm.h"
#include "keyboardled.h"
#include "xt.h"
#include "settings.h"
#include "system.h"
#include "scancode.h"

// repeatState -
// if positive, we're delaying - count up to repeatDelay then go negative
// if negative, we're repeating - count down to repeatRate then return to -1
// if zero, we ain't repeating
__xdata volatile int16_t RepeatState = 0;

__xdata volatile uint8_t RepeatKey = 0x00;
__xdata int16_t RepeatDelay = 7500;
__xdata int16_t RepeatRate = -1000;

__xdata uint8_t LEDDelayMs = 0;

// Mouse buffer needed for handling multi-byte commands and intellimouse detection 
__xdata uint8_t MouseBuffer[MOUSE_BUFFER_SIZE];

// runs in interrupt to keep timings
void RepeatTimer(void)
{
	if (RepeatState > 0)
		RepeatState++;
	else if (RepeatState < 0)
		RepeatState--;
}

__code int16_t DelayConv[] = {
	3750,
	7500,
	11250,
	15000};

__code int16_t RateConv[] = {
	-500,
	-562,
	-625,
	-688,
	-725,
	-811,
	-877,
	-938,
	-1000,
	-1128,
	-1250,
	-1376,
	-1500,
	-1630,
	-1744,
	-1875,
	-2000,
	-2239,
	-2500,
	-2727,
	-3000,
	-3261,
	-3488,
	-3750,
	-4054,
	-4545,
	-5000,
	-5556,
	-6000,
	-6522,
	-7143,
	-7500};

// Runs in main loop
void HandleRepeats(void)
{
	if (RepeatState > RepeatDelay)
	{
		SetRepeatState(-1);
	}
	else if (RepeatState < RepeatRate)
	{
		SendKeyboard( (__code uint8_t *)(FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Make[RepeatKey] : HIDtoSET1_Make[RepeatKey]));
		SetRepeatState(-1);
	}
}


#define SetKey(key,report) (report->KeyboardKeyMap[key >> 3] |= 1 << (key & 0x07))

#define GetOldKey(key,report) (report->oldKeyboardKeyMap[key >> 3] & (1 << (key & 0x07)))

__code uint8_t bitMasks[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1F, 0x3F, 0x7F, 0xFF};

uint16_t BatCounter = 0;

uint32_t SegExtractValue(__xdata HID_SEG *currSeg, __xdata uint8_t *data) {

	uint32_t value = 0;
	uint8_t *currByte;

	// bits may be across any byte alignment
	// so do the neccesary shifting to get it to all fit in an array
	uint8_t shiftbits = (currSeg->startBit % 8);

	uint8_t startbyte = currSeg->startBit / 8;
	
	currByte = data + startbyte;

	int8_t remainingbits = currSeg->reportSize;


	if (shiftbits) {
		// first byte will always be shifted right
		value |= ((uint32_t)(*currByte)) >> (uint32_t)(shiftbits);
		remainingbits -= 8 - shiftbits;
		// everything else will be shifted left by the remaining bits (plus 8 each time)
		shiftbits = 8 - shiftbits;

		currByte++;
	}
	
	// middle bytes keep all 8 bits
	while(remainingbits >= 8) {
		
		value |= (((uint32_t)(*currByte)) ) << (uint32_t)shiftbits;

		currByte++;
		shiftbits += 8;
		remainingbits -= 8;
	}

	//final byte masks off upper bits
	if (remainingbits > 0){
		value |= ((((uint32_t)(*currByte))) & (bitMasks[remainingbits]) ) << (uint32_t)shiftbits;
	}



	return value; 
}

void processSeg(__xdata HID_SEG *currSeg, __xdata HID_REPORT *report, __xdata uint8_t *data)
{
	bool make = 0;
	uint8_t tmp = 0;
	uint16_t cnt, endbit;
	uint8_t *currByte;
	uint8_t pressed = 0;
	int16_t tmpl;
	uint16_t tmpu;

	if (currSeg->InputType == MAP_TYPE_BITFIELD)
	{

		// special case for if we can just copy the whole bitfield into the keyboard buffer
		if (
			(currSeg->OutputChannel == MAP_KEYBOARD)
		)
		{
			cnt = currSeg->startBit & 0x07;
			endbit = currSeg->reportCount & 0x07;
			tmpu = currSeg->OutputControl & 0x07;

			if (cnt == 0 && endbit == 0 && tmpu == 0){
				data += (currSeg->startBit / 8);
				memcpy(report->KeyboardKeyMap + (currSeg->OutputControl / 8), data, (currSeg->reportCount / 8) );
				return;
			}

		}

		endbit = currSeg->startBit + currSeg->reportCount;
		tmp = currSeg->OutputControl;
		for (cnt = currSeg->startBit; cnt < endbit; cnt++)
		{

			pressed = 0;

			// find byte
			currByte = data + ((cnt) >> 3);

			// find bit
			if (*currByte & (0x01 << (cnt & 0x07)))
				pressed = 1;

			if (currSeg->OutputChannel == MAP_KEYBOARD)
			{
				if (pressed)
				{
					SetKey(tmp, report);
					if (!GetOldKey(tmp, report)) {
						report->keyboardUpdated = 1;
					}
				}
				else
				{
					
					if (GetOldKey(tmp, report)) {
						report->keyboardUpdated = 1;
					}
				}
			}
			else
			{
				switch (tmp)
				{
				case MAP_MOUSE_BUTTON1:
					MouseSet(0, pressed);
					break;
				case MAP_MOUSE_BUTTON2:
					MouseSet(1, pressed);
					break;
				case MAP_MOUSE_BUTTON3:
					MouseSet(2, pressed);
					break;
				case MAP_MOUSE_BUTTON4:
					MouseSet(3, pressed);
					break;
				case MAP_MOUSE_BUTTON5:
					MouseSet(4, pressed);
					break;
				}
			}

			tmp++;
		}
	}
	else if (currSeg->InputType) //i.e. not MAP_TYPE_NONE
	{
		uint32_t value = SegExtractValue(currSeg, data);
		
		// if it's a signed integer we need to extend the sign
		if (currSeg->InputParam & INPUT_PARAM_SIGNED)
			value = SIGNEX(value, currSeg->reportSize - 1);

		if (currSeg->OutputChannel == MAP_KEYBOARD)
			report->keyboardUpdated = 1;

		if (currSeg->InputType == MAP_TYPE_THRESHOLD_ABOVE && value > currSeg->InputParam)
		{
			make = 1;
		}
		else if (currSeg->InputType == MAP_TYPE_THRESHOLD_BELOW && value < currSeg->InputParam)
		{
			make = 1;
		}
		else if (currSeg->InputType == MAP_TYPE_EQUAL && value == currSeg->InputParam)
		{
			make = 1;
		}

		else
		{
			make = 0;
		}
		// hack for mouse, as it needs to explicity switch on and off
		// this needs rewritten
		if (currSeg->OutputChannel == MAP_MOUSE && currSeg->InputType == MAP_TYPE_THRESHOLD_ABOVE) {
			switch (currSeg->OutputControl)
				{
				case MAP_MOUSE_BUTTON1:
					MouseSet(0, value);
					break;
				case MAP_MOUSE_BUTTON2:
					MouseSet(1, value);
					break;
				case MAP_MOUSE_BUTTON3:
					MouseSet(2, value);
					break;
				case MAP_MOUSE_BUTTON4:
					MouseSet(3, value);
					break;
				case MAP_MOUSE_BUTTON5:
					MouseSet(4, value);
					break;
				}
		}
		else if (make)
		{
			if (currSeg->OutputChannel == MAP_KEYBOARD)
			{
				SetKey(currSeg->OutputControl, report);
			}
		}
		else if (currSeg->InputType == MAP_TYPE_SCALE)
		{
			if (currSeg->OutputChannel == MAP_MOUSE)
			{

				#define DEADZONE 1

				switch (currSeg->OutputControl)
				{
				// TODO scaling
				case MAP_MOUSE_X:
					if (currSeg->InputParam == INPUT_PARAM_SIGNED_SCALEDOWN){

						tmpl = ((int8_t)((value + 8) >> 4) - 0x08);

						// deadzone
						if (tmpl <= -DEADZONE) tmpl+= DEADZONE;
						else if (tmpl >= DEADZONE) tmpl-= DEADZONE;
						else tmpl = 0;
						
						MouseMove(tmpl, 0, 0);
					}
					else{
						MouseMove((int32_t)value, 0, 0);
					}

					break;
				case MAP_MOUSE_Y:
					if (currSeg->InputParam == INPUT_PARAM_SIGNED_SCALEDOWN) {

						tmpl = ((int8_t)((currSeg->value + 8) >> 4) - 0x08);

						// deadzone
						if (tmpl <= -DEADZONE) tmpl+= DEADZONE;
						else if (tmpl >= DEADZONE) tmpl-= DEADZONE;
						else tmpl = 0;

						MouseMove(0, tmpl, 0);
					}
					else{
						MouseMove(0, (int32_t)value, 0);
					}

					break;
				case MAP_MOUSE_WHEEL:

					MouseMove(0, 0, (int32_t)value);

					break;
				}
			}
		}
		else if (currSeg->InputType == MAP_TYPE_ARRAY)
		{
			if (currSeg->OutputChannel == MAP_KEYBOARD)
			{
				SetKey(value, report);
			}
		}
	}
}

bool BitPresent(uint8_t *bitmap, uint8_t bit)
{
	if (bitmap[bit >> 3] & (1 << (bit & 0x07)))
		return 1;
	else
		return 0;
}
 
bool ParseReport(__xdata INTERFACE *interface, uint32_t len, __xdata uint8_t *report)
{
	__xdata HID_REPORT *descReport;
	__xdata LinkedList *currSegNode;

	// Turn off LEDs for a while
#if defined(BOARD_MICRO)
	SetPWM2Dat(0x00);
#elif defined(BOARD_PS2)
	P0 |= 0b01110000;
#else
	SetPWM1Dat(0x00);
	SetPWM2Dat(0x00);
	T3_FIFO_L = 0;
	T3_FIFO_H = 0;
#endif
	LEDDelayMs = 33;



	if (interface->usesReports)
	{
		// first byte of report will be the report number
		descReport = (__xdata HID_REPORT *)ListGetData(interface->Reports, report[0]);
	}
	else
	{
		descReport = (__xdata HID_REPORT *)ListGetData(interface->Reports, 0);
	}
	
	if (descReport == NULL) {
		DEBUGOUT("Invalid report\n");
		return 0;
	}

	// sanity check length - smaller is no good
	if (len < descReport->length)
	{
		DEBUGOUT("report too short - %lu < %u\n", len, descReport->length);
		return 0;
	}

	currSegNode = descReport->HidSegments;

	// clear key map as all pressed keys should be present in report
	memset(descReport->KeyboardKeyMap, 0, 32);

	while (currSegNode != NULL)
	{
		processSeg((__xdata HID_SEG *)(currSegNode->data), descReport, report);
		currSegNode = currSegNode->next;
	}

	if(descReport->keyboardUpdated)
	{
		// for each byte in the report
		for (uint8_t d = 0; d < 32; d++) 
		{
			// XOR to see if any bits are different
			uint8_t xorred = descReport->KeyboardKeyMap[d] ^ descReport->oldKeyboardKeyMap[d];

			if (xorred) {

				for (uint8_t c = 0; c < 8; c++)
				{
					if (xorred & (1 << c)) 
					{
						uint8_t hidcode = (d << 3) | c;

						if (descReport->KeyboardKeyMap[d] & (1 << c)) // set in current but not prev
						{
							if (MenuActive)
								Menu_Press_Key(hidcode);
							else
							{
								DEBUGOUT("\nSendn %x\n", hidcode);
								// Make

								SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Make[hidcode] : HIDtoSET1_Make[hidcode]);
								if (!(hidcode >= 0xE0 && hidcode <= 0xE7))
								{
									RepeatKey = hidcode;
									SetRepeatState(1);
								}
							}
						}
						else // set in prev but not current
						{
							if (!MenuActive)
							{
								// break

								//DEBUGOUT("\nBreakn %x\n", hidcode);
								// if the key we just released is the one that's repeating then stop
								if (hidcode == RepeatKey)
								{
									RepeatKey = 0;
									SetRepeatState(0);
								}

								// Pause has no break for some reason
								if (hidcode == 0x48)
									continue;

								SendKeyboard(FlashSettings->KeyboardMode == MODE_PS2 ? HIDtoSET2_Break[hidcode] : HIDtoSET1_Break[hidcode]);

							}
						}
					}
					
				}
			}
		}
		memcpy(descReport->oldKeyboardKeyMap, descReport->KeyboardKeyMap, 32);
		descReport->keyboardUpdated = 0;
	}

	return 1;
}

void TypematicDefaults(void)
{
	RepeatDelay = DelayConv[0x01]; // 500ms
	RepeatRate = RateConv[0x0B];   // 10.9cps
}