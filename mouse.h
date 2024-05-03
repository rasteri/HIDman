#include <stdint.h>
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define MOUSE_PORT_PS2 0
#define MOUSE_PORT_SERIAL 1
#define MOUSE_PORT_QUADRATURE 2 //or whatever

#define MOUSE_PS2_TYPE_NORMAL 0
#define MOUSE_PS2_TYPE_INTELLIMOUSE 0

#define MOUSE_PS2_MODE_STREAM 0
#define MOUSE_PS2_MODE_REMOTE 1
#define MOUSE_PS2_MODE_WRAP 2
#define MOUSE_PS2_MODE_RESET 3

#define MOUSE_PS2_RESOLUTION_1CMM 0
#define MOUSE_PS2_RESOLUTION_2CMM 1
#define MOUSE_PS2_RESOLUTION_4CMM 2  
#define MOUSE_PS2_RESOLUTION_8CMM 3

#define MOUSE_PS2_SCALING_1X 0
#define MOUSE_PS2_SCALING_2X 1

#define MOUSE_PS2_REPORTING_OFF 0
#define MOUSE_PS2_REPORTING_ON 1

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

	// ps2 port registers (not relevant for serial port) 
	uint8_t Ps2Type;
	uint8_t Ps2Mode;
	uint8_t Ps2Rate;
	uint8_t Ps2Resolution;
	uint8_t Ps2Scaling;
	uint8_t Ps2DataReporting;
	
} MOUSE;

void InitMice();
void MouseMove(int16_t DeltaX, int16_t DeltaY);
uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, uint8_t *Buttons);
void MouseClick(uint8_t Button);
void MouseUnclick(uint8_t Button);
void MouseSet(uint8_t Button, uint8_t value);

void Ps2MouseSetType(uint8_t Type);
void Ps2MouseSetXY(uint8_t X, uint8_t Y);
void Ps2MouseSetMode(uint8_t Mode);
void Ps2MouseSetRate(uint8_t Rate);
void Ps2MouseSetResolution(uint8_t Resolution);
void Ps2MouseSetScaling(uint8_t Scaling);
void Ps2MouseSetReporting(bool Reporting);
void Ps2MouseSetDefaults(); 

#endif