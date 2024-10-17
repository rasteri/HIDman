#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include "keydefs.h"

/*FOR LATER  -this is how you do BIOS keyboard reads
int main(){

    union REGS r;
    int i;
    for (i = 0; i < 100; i++){
        r.h.ah=0x00;
        printf("%x\n", int86(0x16, &r, &r));
    }


}*/

/*
Really good stuff here - https://github.com/Halicery/8042/blob/main/8042_INTERN.TXT
https://github.com/Halicery/8042/blob/main/8042_1503033.TXT

Options :

* BIOS or direct
* If AT or better, option to read translated scancode set 1 (XLAT = ON) or direct scancode set 2 (XLAT=OFF)
* % coverage of each keyboard type
* media keys - https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-AT-Keyboard-Protocol#media-keys
* mouse with scroll wheel etc? (cutemouse)

*/

char ScreenBuf[0x781];
    

#define TYPE_MAKE 0
#define TYPE_BREAK 1

KeyDef *PrevHighlightKey;
unsigned char PrevOutType;
unsigned int NumDupHighlights;

void HighlightKey(KeyDef *Key, unsigned char outtype)
{
    char x, y;

    gotoxy(14, 24);
    printf("                                                  ");

    gotoxy(14, 24);
    if (outtype == TYPE_MAKE){
        printf("Pressed %s", Key->Name);
        textcolor(WHITE);
        textbackground(LIGHTGRAY);
    }
    else if (outtype == TYPE_BREAK){
        printf("Released %s", Key->Name);
        textcolor(BLACK);
        textbackground(LIGHTGRAY);
    }

    for (x = Key->XPos; x < Key->XPos + Key->Xsize; x++) {
	    for (y = Key->YPos; y < Key->YPos + Key->Ysize; y++) {
            gotoxy(x+1, y+1);
            putch(ScreenBuf[(y*80) + x]);
        }
    }



}

int CompareScanCode(unsigned char *array, unsigned char *s2, unsigned int length) {
    int index = 0;

    // first compare length (first byte of Array)
    if (length != array[0]) 
        return 0;

    //now compare actual codes
    for (index = 0; index < length; index++) {
        if (array[index + 1] != s2[index])
            return 0;
    }
    return 1;
}



KeyDef * FindXTKey(unsigned char *OutType, unsigned char *Scancode, unsigned int length) {
    unsigned int cunt = 0;
    unsigned int index = 0;

    KeyDef *currkeydef;

    for (cunt = 0; cunt < 105; cunt++) {
        currkeydef = &KeyDefs[cunt];

        if (CompareScanCode(currkeydef->XTMake, Scancode, length)){
            *OutType = TYPE_MAKE;
            return currkeydef;
        }
        if (CompareScanCode(currkeydef->XTBreak, Scancode, length)){
            *OutType = TYPE_BREAK;
            return currkeydef;
        }
    }
    return NULL;
}

volatile unsigned char scanbufindex = 0;
volatile unsigned char scancodebuf[16];

static void interrupt keyb_int()
{
    unsigned char code;
    unsigned char cunt;

    unsigned char outtype;

    KeyDef *foundkey;

    scancodebuf[scanbufindex++] = inp(0x60);
    if (scanbufindex >= 16)
        scanbufindex = 0;

    // single E0 scancode ignore
    if (scanbufindex == 1 && scancodebuf[0] == 0xE0) goto endage;

    gotoxy(1, 23);
    printf("Scancode :                                ");
    gotoxy(11, 23);
    for (cunt = 0; cunt < scanbufindex; cunt++)
        printf("%02x ", scancodebuf[cunt]);

    foundkey = FindXTKey(&outtype, scancodebuf, scanbufindex);

    if (foundkey == NULL) {
        printf("(Unknown)");
    }
    else {

        // always clear count
        gotoxy(14, 25);
        printf("       ");

        // if same key as last time, just increment counter and display on screen
        if (foundkey == PrevHighlightKey && outtype == PrevOutType){
            gotoxy(14, 25);
            printf("(x%u)", NumDupHighlights++);
        }
        else {

            NumDupHighlights = 0;
            HighlightKey(foundkey, outtype);
            
            // special case for 2B, also light up Euro1 key (since keycodes are the same)
            if (foundkey->XTMake[1] == 0x2B)
                HighlightKey(&KEYDEF_EURO1, outtype);
        }
        
        PrevHighlightKey = foundkey;
        PrevOutType = outtype;

        scanbufindex = 0;

    }

    endage:

    outp(0x20, 0x20); 

    /*
    static unsigned char buffer;
    unsigned char rawcode;
    unsigned char make_break;
    int scancode;


    make_break = !(rawcode & 0x80); // bit 7: 0 = make, 1 = break 
    scancode = rawcode & 0x7F;

    if (buffer == 0xE0)
    { // second byte of an extended key 
        if (scancode < 0x60)
        {
            extended_keys[scancode] = make_break;
        }
        buffer = 0;
    }
    else if (buffer >= 0xE1 && buffer <= 0xE2)
    {
        buffer = 0; // ingore these extended keys 
    }
    else if (rawcode >= 0xE0 && rawcode <= 0xE2)
    {
        buffer = rawcode; // first byte of an extended key 
    }
    else if (scancode < 0x60)
    {
        normal_keys[scancode] = make_break;
    }

    outp(0x20, 0x20); // must send EOI to finish interrupt 
    */
}

static void interrupt (*old_keyb_int)();

void hook_keyb_int(void)
{
    old_keyb_int = getvect(0x09);
    setvect(0x09, keyb_int);
}

void unhook_keyb_int(void)
{
    if (old_keyb_int != NULL)
    {
        setvect(0x09, old_keyb_int);
        old_keyb_int = NULL;
    }
}

int ctrlbrk_handler(void)
{
    unhook_keyb_int();
    _setcursortype(_NORMALCURSOR);
    return 0;
}



int main()
{
    unsigned char oldscanbufindex = 0;
    unsigned char cunt = 0;
    FILE *keyfile;
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    _setcursortype(_NOCURSOR);
    clrscr();

    keyfile = fopen("keyboard.ans", "r");


    if (fread(ScreenBuf, 0x781, 1, keyfile) != 1) {
        printf("Bugger2\n");
        goto ehoh;
    }

    printf("%s", ScreenBuf);

    /*for (cunt = 0; cunt < 40; cunt++){
        HighlightKey(&KeyDefs[cunt]);
        delay(200);
    }*/
    oldscanbufindex = scanbufindex;
    while(1) {
        scanbufindex = oldscanbufindex;
        delay(100);
        if (scanbufindex == oldscanbufindex)
            scanbufindex = 0;
        delay(100);
    }

    /*while (!normal_keys[1])
    {
	putkeys(1, normal_keys);
	putkeys(2, normal_keys + 0x30);
	putkeys(4, extended_keys);
	putkeys(5, extended_keys + 0x30);
    }*/

    ehoh:

    gotoxy(1, 6);
    _setcursortype(_NORMALCURSOR);
    unhook_keyb_int();
    return 0;
}

/*
1234567890
QWERTYUIOP
ASDFGHJJKL
 ZXCVBNM

*/