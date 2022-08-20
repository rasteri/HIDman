
#ifndef __DATA_H__
#define __DATA_H__
#include <stdbool.h>
#include <stdint.h>

extern __code uint8_t KEY_ACK[];
extern __code uint8_t KEY_BATCOMPLETE[];
extern __code uint8_t KEY_ID[];

extern const uint8_t * const HIDtoPS2_Make[];
extern const uint8_t * const HIDtoPS2_Break[];
extern const uint8_t * const ModtoPS2_BREAK[];
extern const uint8_t * const ModtoPS2_MAKE[];
#endif