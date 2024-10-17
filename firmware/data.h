
#ifndef __DATA_H__
#define __DATA_H__
#include <stdbool.h>
#include <stdint.h>

#include "defs.h"


extern __code uint8_t ASCIItoHID[];

extern __code JoyPreset DefaultJoyMaps[];
extern __code JoyPreset ConfigGameNoMouse[];
extern __code JoyPreset ConfigGameMouse[];

extern __xdata uint8_t StandardKeyboardDescriptor[];
extern __xdata uint8_t StandardMouseDescriptor[];
#endif