/*
	mouse.c
	
    Keeps track of a particular mouse output channel
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "CH559.h"
#include "util.h"
#include "USBHost.h"
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
}

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

    if (m->NeedsUpdating)
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