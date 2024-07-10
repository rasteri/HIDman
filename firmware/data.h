
#ifndef __DATA_H__
#define __DATA_H__
#include <stdbool.h>
#include <stdint.h>

#include "defs.h"

extern __code uint8_t KEY_ACK[];
extern __code uint8_t KEY_BATCOMPLETE[];
extern __code uint8_t KEY_ID[];

extern __code uint8_t MOUSE_ACK[];
extern __code uint8_t MOUSE_BATCOMPLETE[];
extern __code uint8_t MOUSE_ID[];

extern __code uint8_t KEY_LEFT_BREAK[];
extern __code uint8_t KEY_RIGHT_BREAK[];
extern __code uint8_t KEY_UP_BREAK[];
extern __code uint8_t KEY_DOWN_BREAK[];

extern __code uint8_t KEY_LEFT_MAKE[];
extern __code uint8_t KEY_RIGHT_MAKE[];
extern __code uint8_t KEY_UP_MAKE[];
extern __code uint8_t KEY_DOWN_MAKE[];

extern __code uint8_t KEY_LCTRL_MAKE[];
extern __code uint8_t KEY_LALT_MAKE[];
extern __code uint8_t KEY_SPACE_MAKE[];
extern __code uint8_t KEY_ENTER_MAKE[];

extern __code uint8_t KEY_LCTRL_BREAK[];
extern __code uint8_t KEY_LALT_BREAK[];
extern __code uint8_t KEY_SPACE_BREAK[];
extern __code uint8_t KEY_ENTER_BREAK[];

extern __code uint8_t KEY_SCANCODE_2[];
extern __code uint8_t KEY_ECHO[];
extern __code uint8_t KEY_ERROR[];

extern __code uint8_t KEY_LSHIFT_MAKE[];
extern __code uint8_t KEY_LSHIFT_BREAK[];

extern __code uint8_t XT_KEY_LSHIFT_MAKE[];
extern __code uint8_t XT_KEY_LSHIFT_BREAK[];

extern const uint8_t * const HIDtoPS2_Make[];
extern const uint8_t * const HIDtoPS2_Break[];
extern const uint8_t * const ModtoPS2_BREAK[];
extern const uint8_t * const ModtoPS2_MAKE[];

extern const uint8_t * const HIDtoXT_Make[];
extern const uint8_t * const HIDtoXT_Break[];
extern const uint8_t * const ModtoXT_BREAK[];
extern const uint8_t * const ModtoXT_MAKE[];

extern __code uint8_t ASCIItoHID[];

extern __code JoyPreset DefaultJoyMaps[];
extern __code JoyPreset ConfigGameNoMouse[];
extern __code JoyPreset ConfigGameMouse[];

extern __xdata uint8_t StandardKeyboardDescriptor[];
extern __xdata uint8_t StandardMouseDescriptor[];
#endif