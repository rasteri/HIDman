/*
	mouse.c
	
    Keeps track of a particular mouse output channel
*/

#include <stdlib.h>
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
#include "defs.h"
#include "system.h"

#if defined(OPT_SERIAL_MOUSE)
	__xdata uint8_t serialMouseMode = SERIAL_MOUSE_MODE_OFF;
	__xdata char serialMouseType = '3'; // Logitech 3 button: '3', Microsoft: 'M'
#endif

__xdata int16_t Ps2MouseScalingTable[] = {-9, -6, -3, -1, -1, 0, 1, 1, 3, 6, 9};

__xdata MOUSE OutputMice[2];

void InitMice(void)
{
    memset(OutputMice, 0x00, sizeof(OutputMice));
	Ps2MouseSetType(MOUSE_PS2_TYPE_STANDARD);
	Ps2MouseSetDefaults();
}
__xdata uint8_t updates = 0;
void MouseMove(int32_t DeltaX, int32_t DeltaY, int32_t DeltaZ)
{
	if (DeltaX || DeltaY || DeltaZ) {
		for (int x = 0; x < 2; x++)
		{
			MOUSE *m = &OutputMice[x];
			m->DeltaX += DeltaX;
			m->DeltaY += DeltaY;
			m->DeltaZ += DeltaZ;
			m->NeedsUpdating = 1;
		}
	}
}


void GetMouseAxisUpdate(MOUSE *m, int16_t* Axis, int16_t* Value, int16_t Min, int16_t Max, uint8_t Downscale) 
{	
	// assume update value won't exceed min/max limit
	*Value = *Axis >> Downscale;
	
	// but if it does then cap to limit 
	if (*Value < Min)
	{
		*Value = Min;
		m->NeedsUpdating = 1;
	}
	else if (*Value > Max)
	{
		*Value = Max;
		m->NeedsUpdating = 1;
	}	
	
	// decrease delta by update value (delta is zeroed if limits were not exceeded, otherwise we have leftovers)
	// note that we can do power of two downscaling just by two bit shifts, no floating point maths needed
	*Axis -= *Value << Downscale;	
}

uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, int16_t *Z, uint8_t *Buttons, bool Accelerate, uint8_t Downscale)
{
    MOUSE *m = &OutputMice[MouseNo];
	
	/*if (MouseNo == MOUSE_PORT_PS2 && m->Ps2DataReporting == MOUSE_PS2_REPORTING_OFF)
	{
		// ps2 mouse and data reporting is off - no matter if update is needed or not, we do not give one
		return 0;
	}*/
    
	if (m->NeedsUpdating)
    {
        // assume it doesn't need updating after this, but can change if deltas exceeds min/max limit
        m->NeedsUpdating = 0;
		
		// get deltas for x and y (notice downscaling, this is for ps2 mouse resolution but would work with serial as well)
		GetMouseAxisUpdate(m, &m->DeltaX, X, Min, Max, Downscale);
		GetMouseAxisUpdate(m, &m->DeltaY, Y, Min, Max, Downscale);
		
		// get delta for z also for ps2 intellimouse 
		if (MouseNo == MOUSE_PORT_PS2)
			GetMouseAxisUpdate(m, &m->DeltaZ, Z, -8, 7, 0);

		// get buttons
        *Buttons = m->Buttons;
		
		// apply acceleration (this is for ps2 mouse 2:1 scaling support but in theory could be used with serial mouse)
		if (Accelerate)
		{
			*X = (abs(*X) < 6 ? Ps2MouseScalingTable[(*X)+5] : (*X)*2);
			*Y = (abs(*Y) < 6 ? Ps2MouseScalingTable[(*Y)+5] : (*Y)*2);
		}
		
        return 1;
    }
    else
        return 0;
}

void MouseClick(uint8_t Button)
{
    for (int x = 0; x < 2; x++)
    {
        MOUSE *m = &OutputMice[x];
        m->Buttons |= 1 << Button;
        m->NeedsUpdating = 1;
    }
}

void MouseUnclick(uint8_t Button)
{
    for (int x = 0; x < 2; x++)
    {
        MOUSE *m = &OutputMice[x];
        m->Buttons &= ~(1 << Button);
        m->NeedsUpdating = 1;
    }
}

void MouseSet(uint8_t Button, uint8_t value)
{
    for (int x = 0; x < 2; x++)
    {
        MOUSE *m = &OutputMice[x];
        if (value)
            m->Buttons |= 1 << Button;
        else
            m->Buttons &= ~(1 << Button);
        m->NeedsUpdating = 1;
    }
}

void MouseSetAll(uint8_t Buttons)
{
	if (OutputMice[0].Buttons != Buttons) 
	{
		OutputMice[0].Buttons = Buttons;
		OutputMice[0].NeedsUpdating = 1;
	}
	if (OutputMice[1].Buttons != Buttons) 
	{
		OutputMice[1].Buttons = Buttons;
		OutputMice[1].NeedsUpdating = 1;
	}
}

void Ps2MouseSetDelta(uint8_t DeltaX, uint8_t DeltaY, uint8_t DeltaZ)
{
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->DeltaX = DeltaX;
	m->DeltaY = DeltaY;
	m->DeltaZ = DeltaZ;
}

void Ps2MouseSetType(uint8_t Type)
{
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Type = Type;
}

void Ps2MouseSetMode(uint8_t Mode) {
	// TODO: implement (does anything use remote or wrap mode?)
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Mode = Mode;
	Ps2MouseSetDelta(0, 0, 0);
}

void Ps2MouseSetRate(uint8_t Rate) {
	// TODO: implement
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Rate = Rate;
	Ps2MouseSetDelta(0, 0, 0);
}

void Ps2MouseSetResolution(uint8_t Resolution) {
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Resolution = Resolution;
	Ps2MouseSetDelta(0, 0, 0);
}

void Ps2MouseSetScaling(uint8_t Scaling) {
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Scaling = Scaling;
}

void Ps2MouseSetReporting(bool Reporting) {
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2DataReporting = Reporting;
	Ps2MouseSetDelta(0, 0, 0);
}

void Ps2MouseSetDefaults(void) {
	Ps2MouseSetRate(100);
	Ps2MouseSetResolution(MOUSE_PS2_RESOLUTION_8CMM);
	Ps2MouseSetScaling(MOUSE_PS2_SCALING_1X);		
	Ps2MouseSetReporting(MOUSE_PS2_REPORTING_ON);
	Ps2MouseSetMode(MOUSE_PS2_MODE_STREAM);
} 

__xdata uint8_t PrevButtons = 0;

__xdata MOUSE *ps2Mouse = &OutputMice[MOUSE_PORT_PS2];

void HandleMouse(void) {

		int16_t X, Y, Z;
		uint8_t byte1, byte2, byte3, byte4;
		uint8_t Buttons;

		// Send PS/2 Mouse Packet if necessary
		// make sure there's space in the buffer before we pop any mouse updates
		if (((ports[PORT_MOUSE].sendBuffEnd + 1) & 0x07) != ports[PORT_MOUSE].sendBuffStart)
		{
			if (GetMouseUpdate(0, -255, 255, &X, &Y, &Z, &Buttons, (ps2Mouse->Ps2Scaling==MOUSE_PS2_SCALING_2X), (3-ps2Mouse->Ps2Resolution)))
			{
				
				// ps2 is inverted compared to USB
				Y = -Y;

				// TODO: construct bytes from real state
				byte1 = 0b00001000 |			   //bit3 always set
						((Y >> 10) & 0b00100000) | // Y sign bit
						((X >> 11) & 0b00010000) | // X sign bit
						(Buttons & 0x07);

				byte2 = (X & 0xFF);
				byte3 = (Y & 0xFF);


				if (ps2Mouse->Ps2Type == MOUSE_PS2_TYPE_INTELLIMOUSE_3_BUTTON)
				{
					byte4 = (-Z & 0xFF);
					SendMouse4(byte1, byte2, byte3, byte4);
				}
				else if (ps2Mouse->Ps2Type == MOUSE_PS2_TYPE_INTELLIMOUSE_5_BUTTON)
				{
					byte4 = (-Z & 0b00001111) |    // wheel 
					((Buttons << 1) & 0b00110000); // buttons 4 and 5					
					SendMouse4(byte1, byte2, byte3, byte4);
				}
				else
				{
					SendMouse3(byte1, byte2, byte3);
				}
			}
		}

#if defined(OPT_SERIAL_MOUSE)
		if (serialMouseMode == SERIAL_MOUSE_MODE_INIT) {
			// Delay a bit and send 'M' to identify as a mouse
			mDelaymS(5);
			CH559UART1SendByte('M');
			if (serialMouseType != 'M') {
				// Delay a bit longer and send '2' or '3' to further identify # of buttons
				mDelaymS(10);
				CH559UART1SendByte(serialMouseType);
			}
			serialMouseMode = SERIAL_MOUSE_MODE_ACTIVE;
		}
		// Send Serial Mouse Packet if necessary
		// make sure there's space in the fifo before we pop any mouse updates
		else if (serialMouseMode == SERIAL_MOUSE_MODE_ACTIVE && (/*CH559UART1_FIFO_CNT >= 3 || */ SER1_LSR & bLSR_T_FIFO_EMP))
		{
			if (GetMouseUpdate(1, -127, 127, &X, &Y, &Z, &Buttons, false, 0))
			{
				byte1 = 0b11000000 |			  // bit6 always set
						((Buttons & 0x01) << 5) | // left button
						((Buttons & 0x02) << 3) | // right button
						((Y >> 4) & 0b00001100) | // top two bits of Y
						((X >> 6) & 0b00000011);  // top two bits of X

				byte2 = 0b10000000 | (X & 0x3F); // rest of X
				byte3 = 0b10000000 | (Y & 0x3F); // rest of Y

				CH559UART1SendByte(byte1);
				CH559UART1SendByte(byte2);
				CH559UART1SendByte(byte3);

				if (serialMouseType == '3')
				{
					if (Buttons & 0x04)
						CH559UART1SendByte(0b10100000);
					else if (PrevButtons & 0x04)
						CH559UART1SendByte(0b10000000);

					PrevButtons = Buttons;
				}
			}
		}

#endif
}
