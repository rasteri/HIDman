#include <stdlib.h>

typedef struct KD {

    unsigned char *XTMake;
    unsigned char *XTBreak;
    unsigned char *ATMake;
    unsigned char *ATBreak;
    
    char *Name;
    unsigned char *Legend;

    // graphical attrs
    unsigned char XPos, YPos;
    unsigned char Xsize, Ysize;

} KeyDef;

extern KeyDef BIOSKeyDefs[];
#define BIOSKeyDefsSize 2

extern KeyDef KeyDefs[];

extern KeyDef KEYDEF_EURO1;
extern KeyDef KEYDEF_BIOS_F11_MAKE;
extern KeyDef KEYDEF_BIOS_F11_MAKE;

