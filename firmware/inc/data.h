
#ifndef __DATA_H__
#define __DATA_H__
#include <stdbool.h>
#include <stdint.h>

#include "defs.h"


extern __code uint8_t ASCIItoHID[];
extern __code uint8_t QWERTYtoOther[][131];

extern __code JoyPreset DefaultJoyMaps[];
extern __code JoyPreset ConfigGameNoMouse[];
extern __code JoyPreset ConfigGameMouse[];

extern __xdata uint8_t StandardKeyboardDescriptor[];
extern __xdata uint8_t StandardMouseDescriptor[];

extern __code unsigned char KEY_SET2_A_MAKE[];
extern __code unsigned char KEY_SET2_B_MAKE[];
extern __code unsigned char KEY_SET2_C_MAKE[];
extern __code unsigned char KEY_SET2_D_MAKE[];
extern __code unsigned char KEY_SET2_E_MAKE[];
extern __code unsigned char KEY_SET2_F_MAKE[];
extern __code unsigned char KEY_SET2_G_MAKE[];
extern __code unsigned char KEY_SET2_H_MAKE[];
extern __code unsigned char KEY_SET2_I_MAKE[];
extern __code unsigned char KEY_SET2_J_MAKE[];
extern __code unsigned char KEY_SET2_K_MAKE[];
extern __code unsigned char KEY_SET2_L_MAKE[];

extern __code unsigned char KEY_SET2_UP_MAKE[];
extern __code unsigned char KEY_SET2_DOWN_MAKE[];
extern __code unsigned char KEY_SET2_LEFT_MAKE[];
extern __code unsigned char KEY_SET2_RIGHT_MAKE[];

extern __code unsigned char KEY_SET2_A_BREAK[];
extern __code unsigned char KEY_SET2_B_BREAK[];
extern __code unsigned char KEY_SET2_C_BREAK[];
extern __code unsigned char KEY_SET2_D_BREAK[];
extern __code unsigned char KEY_SET2_E_BREAK[];
extern __code unsigned char KEY_SET2_F_BREAK[];
extern __code unsigned char KEY_SET2_G_BREAK[];
extern __code unsigned char KEY_SET2_H_BREAK[];
extern __code unsigned char KEY_SET2_I_BREAK[];
extern __code unsigned char KEY_SET2_J_BREAK[];
extern __code unsigned char KEY_SET2_K_BREAK[];
extern __code unsigned char KEY_SET2_L_BREAK[];
extern __code unsigned char KEY_SET2_UP_BREAK[];
extern __code unsigned char KEY_SET2_DOWN_BREAK[];
extern __code unsigned char KEY_SET2_LEFT_BREAK[];
extern __code unsigned char KEY_SET2_RIGHT_BREAK[];
extern __code unsigned char KEY_SET2_VOLUP_MAKE[];
extern __code unsigned char KEY_SET2_VOLUP_BREAK[];

extern const EXTCHARLOOKUP HID0CtoSET1_Make[];
extern const EXTCHARLOOKUP HID0CtoSET1_Break[];
extern const EXTCHARLOOKUP HID0CtoSET2_Make[];
extern const EXTCHARLOOKUP HID0CtoSET2_Break[];
#endif