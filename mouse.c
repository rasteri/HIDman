/*
	mouse.c
	
    Keeps track of a particular mouse output channel
*/

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

MOUSE OutputMice[2];

void InitMice()
{
    memset(OutputMice, 0x00, sizeof(OutputMice));
	Ps2MouseSetDefaults();
}
uint8_t updates = 0;
void MouseMove(int16_t DeltaX, int16_t DeltaY)
{
    for (int x = 0; x < 2; x++)
    {
        MOUSE *m = &OutputMice[x];
        m->DeltaX += DeltaX;
        m->DeltaY += DeltaY;
        m->NeedsUpdating = 1;
    }
}

uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, uint8_t *Buttons)
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

        *Buttons = m->Buttons;
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

void Ps2MouseSetXY(uint8_t DeltaX, uint8_t DeltaY)
{
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->DeltaX = DeltaX;
	m->DeltaY = DeltaY;
}

void Ps2MouseSetMode(uint8_t Mode) {
	// TODO: implement (does anything use remote or wrap mode?)
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Mode = Mode;
	Ps2MouseSetXY(0, 0);
}

void Ps2MouseSetRate(uint8_t Rate) {
	// TODO: implement
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Rate = Rate;
	Ps2MouseSetXY(0, 0);
}

void Ps2MouseSetResolution(uint8_t Resolution) {
	// TODO: implement
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Resolution = Resolution;
	Ps2MouseSetXY(0, 0);
}

void Ps2MouseSetScaling(uint8_t Scaling) {
	// TODO: implement
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2Scaling = Scaling;
}

void Ps2MouseSetReporting(bool Reporting) {
	MOUSE *m = &OutputMice[MOUSE_PORT_PS2];
	m->Ps2DataReporting = Reporting;
	//m->NeedsUpdating = 1; // looks like some mouse drivers will hang until first update is sent  
	Ps2MouseSetXY(0, 0);
}

void Ps2MouseSetDefaults() {
	Ps2MouseSetRate(100);
	Ps2MouseSetResolution(MOUSE_PS2_RESOLUTION_4CMM);
	Ps2MouseSetScaling(MOUSE_PS2_SCALING_1X);
	Ps2MouseSetReporting(MOUSE_PS2_REPORTING_OFF);
	Ps2MouseSetMode(MOUSE_PS2_MODE_STREAM);
} 
