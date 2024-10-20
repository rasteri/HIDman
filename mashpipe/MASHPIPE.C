

#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include "keydefs.h"

/*
Really good stuff here - https://github.com/Halicery/8042/blob/main/8042_INTERN.TXT
https://github.com/Halicery/8042/blob/main/8042_1503033.TXT
http://vitaly_filatov.tripod.com/ng/asm/asm_027.1.html

Options :

* BIOS or direct
* If AT or better, option to read translated scancode set 1 (XLAT = ON) or direct scancode set 2 (XLAT=OFF)
* % coverage of each keyboard type
* media keys - https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-AT-Keyboard-Protocol#media-keys
* mouse with scroll wheel etc? (cutemouse)

*/

// char SafeWord = {}

char ScreenBuf[0x781];

#define TYPE_MAKE 0
#define TYPE_BREAK 1

#define MODE_RAW 0
#define MODE_BIOS_SIMPLE 1
#define MODE_BIOS_ENHANCED 2

char biosmode = MODE_BIOS_ENHANCED;

KeyDef *PrevHighlightKey;
unsigned char PrevOutType;
unsigned int NumDupHighlights;

KeyDef *codecache[256];
KeyDef *e0codecache[256];
clock_t NextThink = 0;

unsigned char far *text_mem = MK_FP(0xB000, 0x8000);

void BuildCodeCache()
{
    unsigned char cunt;

    KeyDef *currdef;

    memset(codecache, 0x00, sizeof(KeyDef) * 256);
    memset(e0codecache, 0x00, sizeof(KeyDef) * 256);

    for (cunt = 0; cunt < KeyDefsSize; cunt++)
    {

        currdef = &KeyDefs[cunt];

        if (currdef->XTMake[0] == 1)
            codecache[currdef->XTMake[1]] = currdef;

        if (currdef->XTBreak[0] == 1)
            codecache[currdef->XTBreak[1]] = currdef;

        if (currdef->XTMake[0] == 2 && currdef->XTMake[1] == 0xE0)
            e0codecache[currdef->XTMake[2]] = currdef;

        if (currdef->XTBreak[0] == 2 && currdef->XTBreak[1] == 0xE0)
            e0codecache[currdef->XTBreak[2]] = currdef;
    }
}

unsigned char TextClearPattern[] = {
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F,
    0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F, 0x20, 0x0F};

// fast clearline function
void ClearText(unsigned int x, unsigned int y, unsigned int length)
{
   //_fmemset(text_mem + (y * 160) + (2 * x), 0xff, length * 2);
    _fmemcpy(text_mem + (y * 160) + (2 * x), TextClearPattern, length * 2);
}

void HighlightKey(KeyDef *Key, unsigned char outtype, char silent)
{
    char x, y;

    if (outtype == TYPE_MAKE)
    {
        if (!silent)
        {
            ClearText(22, 24, 54);
            gotoxy(14, 25);
            fputs("Pressed  ", stdout);
            fputs(Key->Name, stdout);
        }
        textcolor(WHITE);
        textbackground(LIGHTGRAY);
    }
    else if (outtype == TYPE_BREAK)
    {
        if (!silent)
        {
            ClearText(22, 24, 54);
            gotoxy(14, 25);
            fputs("Released ", stdout);
            fputs(Key->Name, stdout);
        }
        textcolor(BLACK);
        textbackground(LIGHTGRAY);
    }

    for (x = Key->XPos; x < Key->XPos + Key->Xsize; x++)
    {
        for (y = Key->YPos; y < Key->YPos + Key->Ysize; y++)
        {
            gotoxy(x + 1, y + 1);
            putch(ScreenBuf[(y * 80) + x]);
        }
    }
}

int CompareScanCode(unsigned char *array, unsigned char *s2, unsigned int length)
{
    int index = 0;

    // first compare length (first byte of Array)
    if (length != array[0])
        return 0;

    // now compare actual codes
    for (index = 0; index < length; index++)
    {
        if (array[index + 1] != s2[index])
            return 0;
    }
    return 1;
}

char LastFewMakes[4];

KeyDef *SearchList(unsigned char *OutType, KeyDef *list, unsigned int listLength, unsigned char *scancode, unsigned int length)
{

    unsigned int cunt = 0;

    KeyDef *currkeydef;

    for (cunt = 0; cunt < listLength; cunt++)
    {
        currkeydef = &list[cunt];

        if (CompareScanCode(currkeydef->XTMake, scancode, length))
        {
            *OutType = TYPE_MAKE;
            return currkeydef;
        }
        if (!biosmode && CompareScanCode(currkeydef->XTBreak, scancode, length))
        {
            *OutType = TYPE_BREAK;
            return currkeydef;
        }
    }

    return NULL;
}

KeyDef *FindXTKey(unsigned char *OutType, unsigned char *Scancode, unsigned int length)
{

    unsigned int index = 0;

    KeyDef *retval;

    // hack if we're in BIOS mode, check for F11/F12 because they reuse other scancodes
    if (biosmode)
    {
        retval = SearchList(OutType, BIOSKeyDefs, BIOSKeyDefsSize, Scancode, length);
        if (retval != NULL)
            return retval;
    }

    // search the cache first
    if ((length == 1) && (codecache[Scancode[0]] != NULL))
    {
        /*gotoxy(40, 1);
        printf("%X", Scancode[0]);*/
        *OutType = (Scancode[0] & 0x80) ? TYPE_BREAK : TYPE_MAKE;
        return codecache[Scancode[0]];
    }
    if (
        (length == 2) &&
        Scancode[0] == 0xE0 &&
        (codecache[Scancode[1]] != NULL))
    {
        /*gotoxy(40, 1);
        printf("%X", Scancode[0]);*/
        *OutType = (Scancode[1] & 0x80) ? TYPE_BREAK : TYPE_MAKE;
        return e0codecache[Scancode[1]];
    }

    // finally search the whole list
    return SearchList(OutType, KeyDefs, KeyDefsSize, Scancode, length);

    return NULL;
}

volatile unsigned char scanbufindex = 0;
volatile unsigned char scancodebuf[16];

KeyDef *HeldKeys[256];
int HeldKeyIndex = 0;

char exiting = 0;

void ProcessScancode(unsigned char code)
{

    unsigned char cunt;
    unsigned char outtype;
    char sameaslast = 0;

    KeyDef *foundkey;

    // if this is last code in a while,
    // this must be a new scancode
    if (clock() > NextThink)
    {
        //scanbufindex = 0;
        //NextThink = clock() + (CLK_TCK);
    }

    scancodebuf[scanbufindex++] = code;

    if (scanbufindex >= 16)
        scanbufindex = 0;

    // single E0 scancode ignore
    if (scanbufindex == 1 && scancodebuf[0] == 0xE0)
        return;

    ClearText(11, 23, 60);
    gotoxy(12, 24);
    for (cunt = 0; cunt < scanbufindex; cunt++)
        printf("%02X", scancodebuf[cunt]);

    foundkey = FindXTKey(&outtype, scancodebuf, scanbufindex);

    if (foundkey == NULL)
    {
        fputs("(Unknown)", stdout);
    }
    else
    {
        // always clear count
        ClearText(13, 25, 10);
        gotoxy(14, 25);

        if (biosmode)
            HeldKeys[HeldKeyIndex++] = foundkey;

        // if same key as last time, increment counter and display on screen
        if (foundkey == PrevHighlightKey && outtype == PrevOutType)
        {
            sameaslast = 1;
            gotoxy(70, 25);
            printf("(x%u)", ++NumDupHighlights);

            if (NumDupHighlights > 10 && code == 0x01)
            {
                // 10 ESCs in a row, exit
                exiting = 1;
            }
        }
        else
        {
            sameaslast = 0;
            NumDupHighlights = 1;
        }

        HighlightKey(foundkey, outtype, sameaslast);

        // special case for 2B, also light up Euro1 key (since keycodes are the same)
        if (foundkey->XTMake[1] == 0x2B)
            HighlightKey(&KEYDEF_EURO1, outtype, sameaslast);

        PrevHighlightKey = foundkey;
        PrevOutType = outtype;

        scanbufindex = 0;
    }
}

static void interrupt keyb_int()
{
    unsigned char sixone;

    ProcessScancode(inp(0x60));

    // for XT, need to strobe bit7 of 0x61 to acknowledge
    sixone = inp(0x61);
    sixone |= 0x80;
    outp(0x61, sixone);
    sixone &= ~0x80;
    outp(0x61, sixone);

    outp(0x20, 0x20);
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

unsigned char XT_KEY_FWSLASH_TEST[] = {0x35};

#define MOD_RSH 0x01
#define MOD_LSH 0x02
#define MOD_CTRL 0x04
#define MOD_ALT 0x08
#define MOD_SCR 0x10
#define MOD_NUM 0x20
#define MOD_CAPS 0x40
#define MOD_INS 0x80

int main(int argc, char *argv[])
{
    union REGS r;
    FILE *keyfile;
    unsigned char oldscanbufindex = 0;
    unsigned char cunt = 0, shit = 0;
    long conv;

    unsigned char Modifyers = 0, OldModifyers = 0;

    int grabbed;
    unsigned char mask;

    ctrlbrk(ctrlbrk_handler);
    _setcursortype(_NOCURSOR);

    // see if we're on an mda/hercules
    // need a different screen address in that case
    r.h.ah = 0x0F;
    grabbed = int86(0x10, &r, &r);
    if ((grabbed & 0x00FF) == 0x07){
        text_mem = MK_FP(0xB000, 0x0000);
        //printf("MDA Detected %X\n", grabbed);
        //getch();
    }

    if (argc == 2)
    {

        errno = 0;

        conv = strtol(argv[1], NULL, 10);

        if (errno == 0)
        {
            // No error
            biosmode = conv;
        }
    }
    else
    {
        textcolor(WHITE);
        textbackground(BLACK);
        clrscr();
        gotoxy(1, 3);
        cputs("MashPipe v0.0.1\r\n");
        textcolor(LIGHTGRAY);
        cputs("---------------\r\n\r\n");

        cputs("Modes (and what PCs support them)\r\n\r\n");

        textcolor(WHITE);
        cputs("0. Raw Mode (all PCs)\r\n");
        textcolor(LIGHTGRAY);
        cputs("   Reads directly from the keyboard controller at 0x60.\r\n");
        cputs("   Supports all keys/features\r\n\r\n");

        textcolor(WHITE);
        cputs("1. Standard BIOS Mode (all PCs)\r\n");
        textcolor(LIGHTGRAY);
        cputs("   Uses BIOS routines (int16h 00h/01h) to read keyboard.\r\n");
        cputs("   Can't read all keys, or detect when keys released\r\n\r\n");

        textcolor(WHITE);
        cputs("2. Extended BIOS Mode (BIOSes newer than 1985ish)\r\n");
        textcolor(LIGHTGRAY);
        cputs("   Use advanced BIOS routines (int16h 10h/11h) to read keyboard.\r\n");
        cputs("   Reads most keys but still can't detect when released.\r\n\r\n");

        cputs("Not all keys are present on all keyboards.");

        biosmode = getch() - '0';
    }



    clrscr();

    BuildCodeCache();

    keyfile = fopen("keyboard.ans", "r");

    if (fread(ScreenBuf, 0x781, 1, keyfile) != 1)
    {
        printf("Bugger2\n");
        goto ehoh;
    }

    fputs(ScreenBuf, stdout);
    gotoxy(1, 24);
    fputs("Scancode : ", stdout);
    gotoxy(1, 25);
    fputs("Last Action : ", stdout);

    
    // TOP SECRET TEST MODE
    // Lights up all keys
    if (biosmode == 4)
    {
        biosmode = 0;
        for (cunt = 0; cunt < KeyDefsSize; cunt++)
        {
            if (KeyDefs[cunt].XTMake != NULL) 
                for (shit = 0; shit < KeyDefs[cunt].XTMake[0]; shit++)
                    ProcessScancode(KeyDefs[cunt].XTMake[shit + 1]);

            if (KeyDefs[cunt].XTBreak != NULL) 
                for (shit = 0; shit < KeyDefs[cunt].XTBreak[0]; shit++)
                    ProcessScancode(KeyDefs[cunt].XTBreak[shit + 1]);
        }
        getch();
        goto ehoh;
    }

    if (!biosmode)
        hook_keyb_int();

    while (!exiting)
    {
        if (biosmode)
        {
            // see if there's a character waiting
            r.h.ah = 0x01;
            int86(0x16, &r, &r);
            if (!(r.x.flags & 0x0040))
            {
                r.h.ah = 0x00;
                if (biosmode == MODE_BIOS_ENHANCED)
                    r.h.ah = 0x10;

                grabbed = int86(0x16, &r, &r);

                // if LSB is E0, it's extended char, push that first
                if ((grabbed & 0x00FF) == 0x00E0)
                {
                    ProcessScancode(0xE0);
                    ProcessScancode(grabbed >> 8);
                }
                // If MSB is E0, push the LSB also (dumb hack, to avoid a dumb hack in the bios)
                else if ((grabbed & 0xFF00) == 0xE000)
                {
                    ProcessScancode(0xE0);
                    ProcessScancode(grabbed & 0x00FF);
                }
                else
                {
                    ProcessScancode(grabbed >> 8);
                }
                NextThink = clock() + (CLK_TCK / 5);
            }

            // now check modifyer keys
            r.h.ah = 0x02;
            Modifyers = int86(0x16, &r, &r) & 0xFF;

            // XOR to find different bits
            mask = Modifyers ^ OldModifyers;

#define HIGHLIGHTMODIFYER(bit, equivalentscancode) \
    if (mask & bit)                                \
        HighlightKey(codecache[equivalentscancode], Modifyers &bit ? TYPE_MAKE : TYPE_BREAK, 0);

            HIGHLIGHTMODIFYER(MOD_RSH, 0x36);
            HIGHLIGHTMODIFYER(MOD_LSH, 0x2A);
            HIGHLIGHTMODIFYER(MOD_CTRL, 0x1D);
            HIGHLIGHTMODIFYER(MOD_ALT, 0x38);
            HIGHLIGHTMODIFYER(MOD_SCR, 0x46);
            HIGHLIGHTMODIFYER(MOD_NUM, 0x45);
            HIGHLIGHTMODIFYER(MOD_CAPS, 0x3A);
            HIGHLIGHTMODIFYER(MOD_INS, 0x52);

            OldModifyers = Modifyers;

            if (clock() > NextThink)
            {
                for (cunt = 0; cunt < HeldKeyIndex; cunt++)
                {
                    HighlightKey(HeldKeys[cunt], TYPE_BREAK, 1);
                }

                HeldKeyIndex = 0;
                NextThink = 0xFFFFFFFF;
            }
        }
        else
        {
        }
    }

ehoh:
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    clrscr();
    gotoxy(1, 6);
    _setcursortype(_NORMALCURSOR);
    if (!biosmode)
        unhook_keyb_int();
    return 0;
}

/*
1234567890
QWERTYUIOP
ASDFGHJJKL
 ZXCVBNM

*/

// todo : 4&5 triggering F11/F12