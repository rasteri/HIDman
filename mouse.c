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
#include "util.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "menu.h"
#include "mouse.h"

uint8_t Ps2MouseResolutionTable[] = {8, 4, 2, 1};
int16_t Ps2MouseScalingTable[] = {-9, -6, -3, -1, -1, 0, 1, 1, 3, 6, 9};

MOUSE OutputMice[2];

void InitMice()
{
    memset(OutputMice, 0x00, sizeof(OutputMice));
	Ps2MouseSetType(MOUSE_PS2_TYPE_STANDARD);
	Ps2MouseSetDefaults();
}
uint8_t updates = 0;
void MouseMove(int16_t DeltaX, int16_t DeltaY, int16_t DeltaZ)
{
    for (int x = 0; x < 2; x++)
    {
        MOUSE *m = &OutputMice[x];
		if (x == MOUSE_PORT_PS2) {
			// apply resolution for ps2 mouse
			// TODO: can we do this on USB driver level?
			DeltaX /= Ps2MouseResolutionTable[m->Ps2Resolution];
			DeltaY /= Ps2MouseResolutionTable[m->Ps2Resolution];
		}
        m->DeltaX += DeltaX;
        m->DeltaY += DeltaY;
        m->DeltaZ += DeltaZ;
        m->NeedsUpdating = 1;
    }
}

uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, int16_t *Z, uint8_t *Buttons)
{
    MOUSE *m = &OutputMice[MouseNo];
	
	if (MouseNo == MOUSE_PORT_PS2 && m->Ps2DataReporting == MOUSE_PS2_REPORTING_OFF)
	{
		// ps2 mouse and data reporting is off - no matter if update is needed or not, we do not give one
		return 0;
	}
    else if (m->NeedsUpdating)
    {
        // Assume it doesn't need updating after this
        m->NeedsUpdating = 0;

        if (m->DeltaX < Min)
        {
            *X = Min;
            m->DeltaX -= Min;
            m->NeedsUpdating = 1;
        }
        else if (m->DeltaX > Max)
        {
            *X = Max;
            m->DeltaX -= Max;
            m->NeedsUpdating = 1;
        }
        else
        {
            *X = m->DeltaX;
            m->DeltaX = 0;
        }

        if (m->DeltaY < Min)
        {
            *Y = Min;
            m->DeltaY -= Min;
            m->NeedsUpdating = 1;
        }
        else if (m->DeltaY > Max)
        {
            *Y = Max;
            m->DeltaY -= Max;
            m->NeedsUpdating = 1;
        }
        else
        {
            *Y = m->DeltaY;
            m->DeltaY = 0;
        }

		// these limits [-8...7] are for ps2 intellimouse protocol, for serial wheel is not implemented
		if (MouseNo == MOUSE_PORT_PS2)
        {
			if (m->DeltaZ < -8)
			{
				*Z = -8;
				m->DeltaZ -= -8;
				m->NeedsUpdating = 1;
			}
			else if (m->DeltaZ > 7)
			{
				*Z = 7;
				m->DeltaZ -= 7;
				m->NeedsUpdating = 1;
			}
			else
			{
				*Z = m->DeltaZ;
				m->DeltaZ = 0;
			}
        }

        *Buttons = m->Buttons;
		
		if (MouseNo == MOUSE_PORT_PS2 && m->Ps2Scaling == MOUSE_PS2_SCALING_2X)
		{
			// Apply 2:1 scaling for ps2 mouse
			// Note: scaled values should only be reported on automatic reports, not when 
			// report has been requested by host by command 0xEB (remote mode or reporting off)
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

void Ps2MouseSetDefaults() {
	Ps2MouseSetRate(100);
	Ps2MouseSetResolution(MOUSE_PS2_RESOLUTION_4CMM);
	Ps2MouseSetScaling(MOUSE_PS2_SCALING_1X);
	Ps2MouseSetReporting(MOUSE_PS2_REPORTING_OFF);
	Ps2MouseSetMode(MOUSE_PS2_MODE_STREAM);
} 
