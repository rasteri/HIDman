#include <stdlib.h>

typedef struct KD {

    unsigned char *XTMake;
    unsigned char *XTBreak;
    unsigned char *ATMake;
    unsigned char *ATBreak;
    
    char *Name;

    // graphical attrs
    unsigned char XPos, YPos;
    unsigned char Xsize, Ysize;

} KeyDef;

extern KeyDef KeyDefs[];
extern KeyDef KEYDEF_EURO1;