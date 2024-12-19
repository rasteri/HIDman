/*
    MashPipe - Keyboard tester for 8088 + DOS
    Supports all Set 1 scancodes


*/

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

Future expansion :

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

char NAME_AMSTRAD_PADEN[] = "Enter (Keypad/Amstrad)";
char NAME_AMSTRAD_RDEL[] = "Forwardspace (Amstrad)";

// for some reason the bios does weird things here
unsigned char XT_KEY_BIOS_F11_MAKE[] = {1, 0x85};
unsigned char XT_KEY_BIOS_F12_MAKE[] = {1, 0x86};
unsigned char XT_KEY_BIOS_PADEN_MAKE[] = {2, 0xE0, 0x0D};
unsigned char XT_KEY_BIOS_PADFWSLASH_MAKE[] = {2, 0xE0, 0x2F};

char NAME_BIOS_F11[] = "F11 (BIOS)";
char NAME_BIOS_F12[] = "F12 (BIOS)";
char NAME_BIOS_PADEN[] = "Enter (Keypad/BIOS)";
char NAME_BIOS_PADFWSLASH[] = "Forward Slash (Keypad/BIOS)";

KeyDef BIOSKeyDefs[] = {
    {XT_KEY_BIOS_F11_MAKE, NULL, NULL, NULL, NAME_BIOS_F11, NULL, 46, 1, 3, 1 },
    {XT_KEY_BIOS_F12_MAKE, NULL, NULL, NULL, NAME_BIOS_F12, NULL, 50, 1, 3, 1 },
    {NULL, NULL, NULL,NULL,NULL, NULL, NULL,NULL,NULL, NULL}
};

void BuildCodeCache()
{
    unsigned char cunt;

    KeyDef *currdef;

    memset(codecache, 0x00, sizeof(KeyDef) * 256);
    memset(e0codecache, 0x00, sizeof(KeyDef) * 256);

    cunt = 0;
    while (KeyDefs[cunt].Xsize != NULL)
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

        cunt++;
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

/*
│ B3
┤ b4
┐ BF
└ C0
┴ C1
┬ C2
├ C3
─ C4
┼ C5
┘ D9
┌ DA

4bits

LURD
0000 space
0001
0010
0011 ┌ DA
0100
0101 │ B3
0110 └ C0
0111 ├ C3
1000
1001 ┐ BF
1010 ─ C4
1011 ┬ C2
1100 ┘ D9
1101 ┤ b4
1110 ┴ C1
1111 ┼ C5
*/

unsigned char BoxLookup[] = {0x20, NULL, NULL, 0xDA, NULL, 0xB3, 0xC0, 0xC3, NULL, 0xBF, 0xC4, 0xC2, 0xD9, 0xB4, 0xC1, 0xC5};

void DrawBoxChar(unsigned int x, unsigned int y, unsigned char sects)
{
    unsigned char far *screenpt;
    unsigned char far *bufpt;
    unsigned char bitfield;

    x--; y--;

    screenpt = text_mem + (y * 160) + (2 * x);
    bufpt = ScreenBuf + (y * 80) + x;
    
    *bufpt |= sects;
    *screenpt = BoxLookup[*bufpt];
}

void DrawBox(int ax, int ay, int bx, int by)
{
    int tmp;

    // top left corner
    DrawBoxChar(ax, ay, 3);

    // top right corner
    DrawBoxChar(bx, ay, 9);

    // bottom left corner
    DrawBoxChar(ax, by, 6);

    // bottom right corner
    DrawBoxChar(bx, by, 12);

    // top and bottom
    for (tmp = ax+1; tmp < bx; tmp++)
    {
        DrawBoxChar(tmp, ay, 10);
        DrawBoxChar(tmp, by, 10);
    }

    // right and left
    for (tmp = ay+1; tmp < by; tmp++)
    {
        DrawBoxChar(ax, tmp, 5);
        DrawBoxChar(bx, tmp, 5);
    }
}


void DrawKey(KeyDef *key)
{
    unsigned char shit = 0;
    char *pnt;

    DrawBox(key->XPos, key->YPos, key->XPos + key->Xsize + 1, key->YPos + key->Ysize + 1);

    pnt = &(key->Legend[0]);

    gotoxy(key->XPos + 1, key->YPos + 1);

    while (*pnt != '\0')
    {
        if (*pnt == '\n')
        {
            gotoxy(key->XPos + 1, key->YPos + (++shit) + 1);
        }
        else
            putchar(*pnt);
        pnt++;
    }
}

void SetCharAttrs(unsigned int x, unsigned int y, unsigned char attr){
    unsigned char far *screenpt;

    //x--; y--;

    screenpt = text_mem + (y * 160) + (2 * x) + 1;

    *screenpt = attr;
}

void HighlightKey(KeyDef *Key, unsigned char outtype, char silent)
{
    char x, y;
    unsigned char attr;

    if (outtype == TYPE_MAKE)
    {
        //DrawKey(Key);
        if (!silent)
        {
            ClearText(22, 24, 54);
            gotoxy(14, 25);
            fputs("Pressed  ", stdout);
            fputs(Key->Name, stdout);
        }
        //textcolor(WHITE);
        //textbackground(LIGHTGRAY);
        attr = 0x7F;
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
        /*textcolor(BLACK);
        textbackground(LIGHTGRAY);*/

        attr = 0x70;
    }

    for (x = Key->XPos; x < Key->XPos + Key->Xsize; x++)
    {
        for (y = Key->YPos; y < Key->YPos + Key->Ysize; y++)
        {
            SetCharAttrs(x, y, attr);
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

KeyDef *SearchList(unsigned char *OutType, KeyDef *list, unsigned char *scancode, unsigned int length)
{

    unsigned int cunt = 0;

    KeyDef *currkeydef;
    while (list[cunt].Xsize != NULL)
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
        cunt++;
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
        retval = SearchList(OutType, BIOSKeyDefs, Scancode, length);
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
    return SearchList(OutType, KeyDefs, Scancode, length);

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
        // scanbufindex = 0;
        // NextThink = clock() + (CLK_TCK);
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
        /*if (foundkey->XTMake[1] == 0x2B)
            HighlightKey(&KEYDEF_EURO1, outtype, sameaslast);*/

        PrevHighlightKey = foundkey;
        PrevOutType = outtype;

        scanbufindex = 0;
    }
}

// lock access to screen when in interrupt
char ScreenLock;

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

char NAME_BIOS_CAPSLOCK[] = "Caps Lock (BIOS)";
char NAME_BIOS_SCROLLLOCK[] = "Scroll Lock (BIOS)";
char NAME_BIOS_NUMLOCK[] = "Num Lock (BIOS)";
char NAME_BIOS_INSLOCK[] = "Insert (BIOS)";

KeyDef KEY_BIOS_CAPSLOCK = {NULL, NULL, NULL, NULL, NAME_BIOS_CAPSLOCK, NULL, 65, 1, 4, 1};
KeyDef KEY_BIOS_SCROLLLOCK = {NULL, NULL, NULL, NULL, NAME_BIOS_SCROLLLOCK, NULL, 70, 1, 4, 1};
KeyDef KEY_BIOS_NUMLOCK = {NULL, NULL, NULL, NULL, NAME_BIOS_NUMLOCK, NULL, 61, 1, 3, 1};
KeyDef KEY_BIOS_INSLOCK = {NULL, NULL, NULL, NULL, NAME_BIOS_INSLOCK, NULL, 75, 1, 3, 1};

clock_t lastmouseread = 0;
float velx, vely;

#define FILTERALPHA 0.2

void ReadMouse() {
    union REGS r;
    int grabbed;
    clock_t currtime;
    int clockvelx, clockvely;



    currtime = clock();

    if (currtime > lastmouseread) {
        //read buttons
        r.x.ax = 0x03;
        grabbed = int86(0x33, &r, &r);
        gotoxy(62, 23);
        printf("%04X", r.x.bx);

        // read relative mouse movement
        r.x.ax = 0x0B;
        grabbed = int86(0x33, &r, &r);

        // reread clock so we're accurate
        currtime = clock();

        clockvelx = (int)r.x.cx;
        clockvely = (int)r.x.dx;

        velx = (FILTERALPHA * ((float)clockvelx / (float)(currtime - lastmouseread))) + ((1-FILTERALPHA) * velx);
        vely = (FILTERALPHA * ((float)clockvely / (float)(currtime - lastmouseread))) + ((1-FILTERALPHA) * vely);

        gotoxy(67, 23);
        printf("%4.0f %4.0f  ", velx, vely);
        lastmouseread = currtime;
    }

}

int main(int argc, char *argv[])
{
    union REGS r;
    FILE *keyfile;
    unsigned char oldscanbufindex = 0;
    unsigned char cunt = 0, shit = 0;
    long conv;

    char *pnt;

    unsigned char Modifyers = 0, OldModifyers = 0;

    int grabbed;
    unsigned char mask;

    ctrlbrk(ctrlbrk_handler);
    _setcursortype(_NOCURSOR);

    // see if we're on an mda/hercules
    // need a different screen address in that case
    r.h.ah = 0x0F;
    grabbed = int86(0x10, &r, &r);
    if ((grabbed & 0x00FF) == 0x07)
    {
        text_mem = MK_FP(0xB000, 0x0000);
        // printf("MDA Detected %X\n", grabbed);
        // getch();
    }

    clrscr();

    /*for (cunt = 0; cunt < KeyDefsSize; cunt++)
    {
        printf(KeyDefs[cunt].Name);
        printf(",");
        pnt = KeyDefs[cunt].Legend;
        while (*pnt){
            printf("%02X ", *pnt++);
        }
        printf(", %d, %d, %d, %d, ", KeyDefs[cunt].XPos, KeyDefs[cunt].YPos, KeyDefs[cunt].Xsize, KeyDefs[cunt].Ysize);
        pnt = KeyDefs[cunt].Legend;
        while (*pnt){
            printf("%02X ", *pnt++);
        }
        printf("\n");

    }
    exit(0);*/
    _fmemset(ScreenBuf, 0x00, 0x781);

    r.x.ax = 0x01;
    grabbed = int86(0x33, &r, &r);

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

    _fmemset(ScreenBuf, 0x00, 0x781);

    // Draw the keys
    cunt = 0;

    while (KeyDefs[cunt].Xsize != NULL)
    {
        DrawKey(&KeyDefs[cunt]);
        cunt++;
    }

    gotoxy(59, 1);
    fputs("Locks:", stdout);

    gotoxy(62, 2);
    fputs("NUM CAPS SCRL INS", stdout);

    gotoxy(57, 5);
    fputs("Media:", stdout);

    gotoxy(62, 11);
    fputs("WWW:", stdout);

    gotoxy(62, 18);
    fputs("Sys:", stdout);

    gotoxy(64, 21);
    fputs("Hold ESC to exit", stdout);

    gotoxy(1, 24);
    fputs("Scancode : ", stdout);

    gotoxy(1, 25);
    fputs("Last Action : ", stdout);

    // TOP SECRET TEST MODE
    // Lights up all keys
    if (biosmode == 4)
    {
        biosmode = 0;
        cunt = 0;
        while(KeyDefs[cunt].Xsize != NULL)
        {
            if (KeyDefs[cunt].XTMake != NULL)
                for (shit = 0; shit < KeyDefs[cunt].XTMake[0]; shit++)
                    ProcessScancode(KeyDefs[cunt].XTMake[shit + 1]);

            if (KeyDefs[cunt].XTBreak != NULL)
                for (shit = 0; shit < KeyDefs[cunt].XTBreak[0]; shit++)
                    ProcessScancode(KeyDefs[cunt].XTBreak[shit + 1]);
            
            cunt ++;
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

#define HIGHLIGHTLOCK(bit, keyd) \
    if (mask & bit)              \
        HighlightKey(keyd, Modifyers &bit ? TYPE_MAKE : TYPE_BREAK, 0);

            HIGHLIGHTLOCK(MOD_SCR, &KEY_BIOS_SCROLLLOCK);
            HIGHLIGHTLOCK(MOD_NUM, &KEY_BIOS_NUMLOCK);
            HIGHLIGHTLOCK(MOD_CAPS, &KEY_BIOS_CAPSLOCK);
            HIGHLIGHTLOCK(MOD_INS, &KEY_BIOS_INSLOCK);

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

            ReadMouse();
        }
        else
        {
        }
    }

ehoh:
    // Hide mouse cursor
    r.x.ax = 0x02;
    grabbed = int86(0x33, &r, &r);

    textcolor(LIGHTGRAY);
    textbackground(BLACK);
    clrscr();
    gotoxy(1, 6);
    _setcursortype(_NORMALCURSOR);
    if (!biosmode)
        unhook_keyb_int();
    return 0;
}
