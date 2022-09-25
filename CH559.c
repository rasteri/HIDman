#include "Type.h"
#include "CH559.h"

__at (0x2446) UINT8XV  UEP4_1_MOD  ;   // endpoint 4/1 mode
__at (0x2546) UINT8PV pUEP4_1_MOD  ;
__at (0x2447) UINT8XV  UEP2_3_MOD  ;   // endpoint 2/3 mode
__at (0x2547) UINT8PV pUEP2_3_MOD  ;
__at (0x2448) UINT8XV  UEP0_DMA_H  ;   // endpoint 0&4 buffer start address high byte
__at (0x2449) UINT8XV  UEP0_DMA_L  ;   // endpoint 0&4 buffer start address low byte
__at (0x244A) UINT8XV  UEP1_DMA_H  ;   // endpoint 1 buffer start address high byte
__at (0x244B) UINT8XV  UEP1_DMA_L  ;   // endpoint 1 buffer start address low byte
__at (0x244C) UINT8XV  UEP2_DMA_H  ;   // endpoint 2 buffer start address high byte
__at (0x244D) UINT8XV  UEP2_DMA_L  ;   // endpoint 2 buffer start address low byte
__at (0x244E) UINT8XV  UEP3_DMA_H  ;   // endpoint 3 buffer start address high byte
__at (0x244F) UINT8XV  UEP3_DMA_L  ;   // endpoint 3 buffer start address low byte
__at (0x2880) UINT8XV  LED_STAT    ;   // LED status
__at (0x2980) UINT8PV pLED_STAT    ;
__at (0x2881) UINT8XV  LED_CTRL    ;   // LED control
__at (0x2981) UINT8PV pLED_CTRL    ;
__at (0x2882) UINT8XV  LED_DATA    ;   // WriteOnly: data port
__at (0x2982) UINT8PV pLED_DATA    ;
__at (0x2883) UINT8XV  LED_CK_SE   ;   // clock divisor setting
__at (0x2983) UINT8PV pLED_CK_SE   ;
__at (0x2884) UINT8XV  LED_DMA_AH  ;   // DMA address high byte, automatic increasing after DMA
__at (0x2885) UINT8XV  LED_DMA_AL  ;   // DMA address low byte, automatic increasing after DMA
__at (0x2984) UINT8PV pLED_DMA_AH  ;
__at (0x2985) UINT8PV pLED_DMA_AL  ;
__at (0x2886) UINT8XV  LED_DMA_CN  ;   // DMA remainder word count, just main buffer and exclude aux buffer, automatic decreasing after DMA
__at (0x2986) UINT8PV pLED_DMA_CN  ;
__at (0x2888) UINT8XV  LED_DMA_XH  ;   // aux buffer DMA address high byte, automatic increasing after DMA
__at (0x2889) UINT8XV  LED_DMA_XL  ;   // aux buffer DMA address low byte, automatic increasing after DMA
__at (0x2988) UINT8PV pLED_DMA_XH  ;
__at (0x2989) UINT8PV pLED_DMA_XL  ;

