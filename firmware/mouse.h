#include <stdint.h>
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define MOUSE_PORT_PS2 0
#define MOUSE_PORT_SERIAL 1
#define MOUSE_PORT_QUADRATURE 2 //or whatever

#define MOUSE_PS2_TYPE_STANDARD 0
#define MOUSE_PS2_TYPE_INTELLIMOUSE_3_BUTTON 3
#define MOUSE_PS2_TYPE_INTELLIMOUSE_5_BUTTON 4

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

#define SERIAL_MOUSE_MODE_OFF    0
#define SERIAL_MOUSE_MODE_RESET  1
#define SERIAL_MOUSE_MODE_INIT   2
#define SERIAL_MOUSE_MODE_ACTIVE 3

typedef struct MOUSE {

    // the accumulated X, Y and wheel movements
    // may have been accumulated over several USB polls
    // and may require several packets to send on the output device
    int16_t DeltaX, DeltaY, DeltaZ;

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

extern MOUSE OutputMice[];

void InitMice(void);
void MouseMove(int32_t DeltaX, int32_t DeltaY, int32_t DeltaZ);
uint8_t GetMouseUpdate(uint8_t MouseNo, int16_t Min, int16_t Max, int16_t *X, int16_t *Y, int16_t *Z, uint8_t *Buttons, bool Accelerate, uint8_t Downscale);
void MouseClick(uint8_t Button);
void MouseUnclick(uint8_t Button);
void MouseSet(uint8_t Button, uint8_t value);

void Ps2MouseSetDelta(uint8_t DeltaX, uint8_t DeltaY, uint8_t DeltaZ);
void Ps2MouseSetType(uint8_t Type);
void Ps2MouseSetMode(uint8_t Mode);
void Ps2MouseSetRate(uint8_t Rate);
void Ps2MouseSetResolution(uint8_t Resolution);
void Ps2MouseSetScaling(uint8_t Scaling);
void Ps2MouseSetReporting(bool Reporting);
void Ps2MouseSetDefaults(void); 

extern uint8_t serialMouseMode;

void HandleMouse(void);

#endif