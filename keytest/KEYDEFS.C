#include "keydefs.h"
#include "..\firmware\scancode.c"

char NAME_A[] = "A";
char NAME_B[] = "B";
char NAME_C[] = "C";
char NAME_D[] = "D";
char NAME_E[] = "E";
char NAME_F[] = "F";
char NAME_G[] = "G";
char NAME_H[] = "H";
char NAME_I[] = "I";
char NAME_J[] = "J";
char NAME_K[] = "K";
char NAME_L[] = "L";
char NAME_M[] = "M";
char NAME_N[] = "N";
char NAME_O[] = "O";
char NAME_P[] = "P";
char NAME_Q[] = "Q";
char NAME_R[] = "R";
char NAME_S[] = "S";
char NAME_T[] = "T";
char NAME_U[] = "U";
char NAME_V[] = "V";
char NAME_W[] = "W";
char NAME_X[] = "X";
char NAME_Y[] = "Y";
char NAME_Z[] = "Z";
char NAME_0[] = "Zero / Right Round Bracket (Main)";
char NAME_1[] = "One / Exclamation Mark (Main)";
char NAME_2[] = "Two / At Symbol (Main)";
char NAME_3[] = "Three / Hash (Main)";
char NAME_4[] = "Four / Dollar Sign (Main)";
char NAME_5[] = "Five / Percent Sign (Main)";
char NAME_6[] = "Six / Caret (Main)";
char NAME_7[] = "Seven / Ampersand (Main)";
char NAME_8[] = "Eight / Asterisk (Main)";
char NAME_9[] = "Nine / Left Round Bracket (Main)";
char NAME_TILDE[] = "Backquote/Tilde";
char NAME_DASH[] = "Dash/Underscore";
char NAME_EQUAL[] = "Equals/Plus (Main)";
char NAME_BKSLASH[] = "Backslash/Pipe (US), or Hash/Tilde (Europe)";
char NAME_BKSPACE[] = "Backspace";
char NAME_SPACE[] = "Space";
char NAME_TAB[] = "Tab";
char NAME_CAPS[] = "Caps Lock";
char NAME_LSHIFT[] = "Shift (Left)";
char NAME_LCTRL[] = "Control (Left)";
char NAME_LGUI[] = "GUI (Left)";
char NAME_LALT[] = "Alt (Left)";
char NAME_RSHIFT[] = "Shift (Right)";
char NAME_RCTRL[] = "Control (Right)";
char NAME_RGUI[] = "GUI (Right)";
char NAME_RALT[] = "Alt (Right)";
char NAME_APPS[] = "Apps";
char NAME_ENTER[] = "Enter (Main)";
char NAME_ESC[] = "Escape";
char NAME_F1[] = "F1";
char NAME_F2[] = "F2";
char NAME_F3[] = "F3";
char NAME_F4[] = "F4";
char NAME_F5[] = "F5";
char NAME_F6[] = "F6";
char NAME_F7[] = "F7";
char NAME_F8[] = "F8";
char NAME_F9[] = "F9";
char NAME_F10[] = "F10";
char NAME_F11[] = "F11";
char NAME_F12[] = "F12";
char NAME_PRTSC[] = "Print Screen/SysRq";
char NAME_SCROLL[] = "Scroll Lock";
char NAME_PAUSE[] = "Pause/Break";
char NAME_LEFTSQB[] = "Left Square/Curly Bracket";
char NAME_INSERT[] = "Insert (Main)";
char NAME_HOME[] = "Home (Main)";
char NAME_PGUP[] = "Page Up (Main)";
char NAME_DELETE[] = "Delete (Main)";
char NAME_END[] = "End (Main)";
char NAME_PGDN[] = "Page Down (Main)";
char NAME_UP[] = "Up Arrow (Main)";
char NAME_LEFT[] = "Left Arrow (Main)";
char NAME_DOWN[] = "Down Arrow (Main)";
char NAME_RIGHT[] = "Right Arrow (Main)";
char NAME_NUM[] = "Num Lock";
char NAME_PADFWSLASH[] = "Forward Slash (Keypad)";
char NAME_PADASTERISK[] = "Asterisk (Keypad)";
char NAME_PADMINUS[] = "Minus (Keypad)";
char NAME_PADPLUS[] = "Plus (Keypad)";
char NAME_PADEN[] = "Enter (Keypad)";
char NAME_PADDOT[] = "Period/Delete (Keypad)";
char NAME_PAD0[] = "Zero/Insert (Keypad)";
char NAME_PAD1[] = "One/End (Keypad)";
char NAME_PAD2[] = "Two/Downarrow (Keypad)";
char NAME_PAD3[] = "Three/Page Down (Keypad)";
char NAME_PAD4[] = "Four/Left Arrow (Keypad)";
char NAME_PAD5[] = "Five (Keypad)";
char NAME_PAD6[] = "Six/Right Arrow (Keypad)";
char NAME_PAD7[] = "Seven/Home (Keypad)";
char NAME_PAD8[] = "Eight/Up Arrow (Keypad)";
char NAME_PAD9[] = "Nine/Page Up (Keypad)";
char NAME_PADCOMMA[] = "Comma (Keypad)";
char NAME_RIGHTSQB[] = "Left Square/Curly Bracket";
char NAME_SEMICOLON[] = "Semicolon/Colon";
char NAME_APOSTROPHE[] = "Apostrophe/At Symbol";
char NAME_COMMA[] = "Comma/Left Angle Bracket";
char NAME_PERIOD[] = "Period/Right Angle Bracket";
char NAME_FWSLASH[] = "Forward Slash/Question Mark";
char NAME_EURO1[] = "Backslash/Pipe (US), or Hash/Tilde (Europe)";
char NAME_EURO2[] = "Backslash/Pipe (Europe)";
char NAME_BIOS_F11[] = "F11 (BIOS)";
char NAME_BIOS_F12[] = "F12 (BIOS)";
char NAME_BIOS_PADEN[] = "Enter (Keypad, BIOS)";
char NAME_BIOS_PADFWSLASH[] = "Forward Slash (Keypad, BIOS)";

#define CNCAT2(a,b) a##b
#define CNCAT3(a,b,c) a##b##c

#define DEFINEKEY(keyname,x,y,sx,sy) { CNCAT3(XT_KEY_,keyname,_MAKE), CNCAT3(XT_KEY_,keyname,_BREAK), CNCAT3(KEY_,keyname,_MAKE), CNCAT3(KEY_,keyname,_BREAK), CNCAT3(NAME_,keyname,),x,y,sx,sy}

KeyDef KEYDEF_EURO1 = DEFINEKEY(EURO1, 28, 10, 1, 2);

KeyDef KeyDefs[] = {

DEFINEKEY(A, 6, 10, 1, 2),
DEFINEKEY(B, 15, 13, 1, 2 ),
DEFINEKEY(C, 11, 13, 1, 2 ),
DEFINEKEY(D, 10, 10, 1, 2 ),
DEFINEKEY(E, 9, 7, 1, 2 ),
DEFINEKEY(F, 12, 10, 1, 2 ),
DEFINEKEY(G, 14, 10, 1, 2 ),
DEFINEKEY(H, 16, 10, 1, 2 ),
DEFINEKEY(I, 19, 7, 1, 2 ),
DEFINEKEY(J, 18, 10, 1, 2 ),
DEFINEKEY(K, 20, 10, 1, 2 ),
DEFINEKEY(L, 22, 10, 1, 2 ),
DEFINEKEY(M, 19, 13, 1, 2 ),
DEFINEKEY(N, 17, 13, 1, 2 ),
DEFINEKEY(O, 21, 7, 1, 2 ),
DEFINEKEY(P, 23, 7, 1, 2 ),
DEFINEKEY(Q, 5, 7, 1, 2 ),
DEFINEKEY(R, 11, 7, 1, 2 ),
DEFINEKEY(S, 8, 10, 1, 2 ),
DEFINEKEY(T, 13, 7, 1, 2 ),
DEFINEKEY(U, 17, 7, 1, 2 ),
DEFINEKEY(V, 13, 13, 1, 2 ),
DEFINEKEY(W, 7, 7, 1, 2 ),
DEFINEKEY(X, 9, 13, 1, 2 ),
DEFINEKEY(Y, 15, 7, 1, 2 ),
DEFINEKEY(Z, 7, 13, 1, 2 ),
DEFINEKEY(1, 4, 4, 1, 2 ),
DEFINEKEY(2, 6, 4, 1, 2 ),
DEFINEKEY(3, 8, 4, 1, 2 ),
DEFINEKEY(4, 10, 4, 1, 2 ),
DEFINEKEY(5, 12, 4, 1, 2 ),
DEFINEKEY(6, 14, 4, 1, 2 ),
DEFINEKEY(7, 16, 4, 1, 2 ),
DEFINEKEY(8, 18, 4, 1, 2 ),
DEFINEKEY(9, 20, 4, 1, 2 ),
DEFINEKEY(0, 22, 4, 1, 2 ),
DEFINEKEY(TILDE, 1, 4, 2, 2 ),
DEFINEKEY(DASH, 24, 4, 1, 2 ),
DEFINEKEY(EQUAL, 26, 4, 1, 2 ),
DEFINEKEY(BKSLASH, 29, 7, 1, 2 ),
DEFINEKEY(BKSPACE, 28, 4, 5, 2 ),
DEFINEKEY(SPACE, 13, 16, 3, 1 ),
DEFINEKEY(TAB, 1, 7, 3, 2 ),
DEFINEKEY(CAPS, 1, 10, 4, 2 ),
DEFINEKEY(LSHIFT, 1, 13, 3, 2 ),
DEFINEKEY(LCTRL, 1, 16, 3, 1 ),
DEFINEKEY(LGUI, 5, 16, 3, 1 ),
DEFINEKEY(LALT, 9, 16, 3, 1 ),
DEFINEKEY(RSHIFT, 27, 13, 5, 2),
DEFINEKEY(RCTRL, 29, 16, 4, 1 ),
DEFINEKEY(RGUI, 21, 16, 3, 1 ),
DEFINEKEY(RALT, 17, 16, 3, 1 ),
DEFINEKEY(APPS, 25, 16, 3, 1 ),
DEFINEKEY(ENTER, 30, 10, 3, 2 ),
DEFINEKEY(ESC, 1, 1, 3, 1 ),
DEFINEKEY(F1, 6, 1, 3, 1 ),
DEFINEKEY(F2, 10, 1, 3, 1 ),
DEFINEKEY(F3, 14, 1, 3, 1 ),
DEFINEKEY(F4, 18, 1, 3, 1 ),
DEFINEKEY(F5, 22, 1, 3, 1 ),
DEFINEKEY(F6, 26, 1, 3, 1 ),
DEFINEKEY(F7, 30, 1, 3, 1 ),
DEFINEKEY(F8, 34, 1, 3, 1 ),
DEFINEKEY(F9, 38, 1, 3, 1 ),
DEFINEKEY(F10, 42, 1, 3, 1 ),
DEFINEKEY(F11, 46, 1, 3, 1 ),
DEFINEKEY(F12, 50, 1, 3, 1 ),
DEFINEKEY(PRTSC, 35, 4, 3, 2),
DEFINEKEY(SCROLL, 39, 4, 3, 2 ),
DEFINEKEY(PAUSE, 43, 4, 3, 2 ),
DEFINEKEY(LEFTSQB, 25, 7, 1, 2 ),
DEFINEKEY(INSERT, 35, 7, 3, 2 ),
DEFINEKEY(HOME, 39, 7, 3, 2 ),
DEFINEKEY(PGUP, 43, 7, 3, 2 ),
DEFINEKEY(DELETE, 35, 10, 3, 2 ),
DEFINEKEY(END, 39, 10, 3, 2 ),
DEFINEKEY(PGDN, 43, 10, 3, 2 ),
DEFINEKEY(UP, 39, 14, 3, 1 ),
DEFINEKEY(LEFT, 35, 16, 3, 1 ),
DEFINEKEY(DOWN, 39, 16, 3, 1 ),
DEFINEKEY(RIGHT, 43, 16, 3, 1 ),
DEFINEKEY(NUM, 48, 4, 2, 1 ),
DEFINEKEY(PADFWSLASH, 51, 4, 2, 1 ),
DEFINEKEY(PADASTERISK, 54, 4, 2, 1 ),
DEFINEKEY(PADMINUS, 57, 4, 2, 1 ),
DEFINEKEY(PADPLUS, 57, 6, 2, 2 ),
DEFINEKEY(PADEN, 57, 12, 2, 5 ),
DEFINEKEY(PADDOT, 54, 15, 2, 2 ),
DEFINEKEY(PAD0, 48, 15, 5, 2 ),
DEFINEKEY(PAD1, 48, 12, 2, 2 ),
DEFINEKEY(PAD2, 51, 12, 2, 2 ),
DEFINEKEY(PAD3, 54, 12, 2, 2 ),
DEFINEKEY(PAD4, 48, 9, 2, 2 ),
DEFINEKEY(PAD5, 51, 9, 2, 2 ),
DEFINEKEY(PAD6, 54, 9, 2, 2 ),
DEFINEKEY(PAD7, 48, 6, 2, 2 ),
DEFINEKEY(PAD8, 51, 6, 2, 2 ),
DEFINEKEY(PAD9, 54, 6, 2, 2 ),
DEFINEKEY(RIGHTSQB, 27, 7, 1, 2 ),
DEFINEKEY(SEMICOLON, 24, 10, 1, 2 ),
DEFINEKEY(APOSTROPHE, 26, 10, 1, 2 ),
DEFINEKEY(COMMA, 21, 13, 1, 2 ),
DEFINEKEY(PERIOD, 23, 13, 1, 2 ),
DEFINEKEY(FWSLASH, 25, 13, 1, 2 ),
DEFINEKEY(EURO2, 5, 13, 1, 2 ),
DEFINEKEY(EURO2, 5, 13, 1, 2 ),
{XT_KEY_BIOS_F11_MAKE, NULL, NULL, NULL, NAME_BIOS_F11, 46, 1, 3, 1 },
{XT_KEY_BIOS_F12_MAKE, NULL, NULL, NULL, NAME_BIOS_F12, 50, 1, 3, 1 },
{XT_KEY_BIOS_PADEN_MAKE, NULL, NULL, NULL, NAME_BIOS_PADEN, 57, 12, 2, 5 },
{XT_KEY_BIOS_PADFWSLASH_MAKE, NULL, NULL, NULL, NAME_BIOS_PADFWSLASH, 51, 4, 2, 1 }
};