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


KeyDef * FindXTKey(unsigned char *Scancode) {

}

static void interrupt keyb_int()
{
    /*
    static unsigned char buffer;
    unsigned char rawcode;
    unsigned char make_break;
    int scancode;

    rawcode = inp(0x60);            // read scancode from keyboard controller 
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

char ScreenBuf[0x781];
    
void HighlightKey(KeyDef *Key)
{
    char x, y;

    textcolor(BLACK);
    textbackground(WHITE);

    for (x = Key->XPos; x < Key->XPos + Key->Xsize; x++) {
	    for (y = Key->YPos; y < Key->YPos + Key->Ysize; y++) {
            gotoxy(x+1, y+1);
            putch(ScreenBuf[(y*80) + x]);
        }
    }
}

int main()
{
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

    for (cunt = 0; cunt < 40; cunt++){
        HighlightKey(&KeyDefs[cunt]);
    }

goto ehoh;

    /*while (!normal_keys[1])
    {
	putkeys(1, normal_keys);
	putkeys(2, normal_keys + 0x30);
	putkeys(4, extended_keys);
	putkeys(5, extended_keys + 0x30);
    }*/

    ehoh:

    //gotoxy(1, 6);
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