#include <stdint.h>
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define MOUSE_PORT_PS2 0
#define MOUSE_PORT_SERIAL 1
#define MOUSE_PORT_QUADRATURE 2 //or whatever

typedef struct MOUSE {

    // the accumulated X and Y movements
    // may have been accumulated over several USB polls
    // and may require several packets to send on the output device
    int16_t DeltaX, DeltaY;

    // bitmap with currently held buttons
    uint8_t Buttons;

    // do we need to send something?
    // set by USB HID processing, cleared by output
    bool NeedsUpdating;

} MOUSE;

void InitMice();
void MouseMove(int16_t DeltaX, int16_t DeltaY);
uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, uint8_t *Buttons);
void MouseClick(uint8_t Button);
void MouseUnclick(uint8_t Button);
void MouseSet(uint8_t Button, uint8_t value);
#endif