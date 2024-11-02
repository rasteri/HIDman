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
char NAME_RIGHTSQB[] = "Right Square/Curly Bracket";
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
char NAME_SYSPOWER[] = "System Power";
char NAME_SYSSLEEP[] = "System Sleep";
char NAME_SYSWAKE[] = "System Wake";
char NAME_PADEQUAL[] = "Keypad Equals";
char NAME_F13[] = "F13";
char NAME_F14[] = "F14";
char NAME_F15[] = "F15";
char NAME_F16[] = "F16";
char NAME_F17[] = "F17";
char NAME_F18[] = "F18";
char NAME_F19[] = "F19";
char NAME_F20[] = "F20";
char NAME_F21[] = "F21";
char NAME_F22[] = "F22";
char NAME_F23[] = "F23";
char NAME_F24[] = "F24";
char NAME_INTL1[] = "Ro";
char NAME_INTL2[] = "Katakana/Hiragana OR Forwardspace (AMSTRAD)";
char NAME_INTL3[] = "Yen";
char NAME_INTL4[] = "Henkan";
char NAME_INTL5[] = "Muhenkan";
char NAME_INTL6[] = "PC9800 Comma";
char NAME_LANG1[] = "Hangul/Yeong";
char NAME_LANG2[] = "Hanja";
char NAME_LANG3[] = "Katakana";
char NAME_LANG4[] = "Hiragana";
char NAME_LANG5[] = "Zenkaku/Hankaku/Kanji";
char NAME_MEDIANEXT[] = "Media Next Track";
char NAME_MEDIAPREV[] = "Media Previous Track";
char NAME_MEDIASTOP[] = "Media Stop";
char NAME_MEDIAPLAY[] = "Media Play/Pause";
char NAME_MEDIAMUTE[] = "Media Mute";
char NAME_MEDIAVOLUP[] = "Media Volume Up";
char NAME_MEDIAVOLDN[] = "Media Volume Down";
char NAME_MEDIASEL[] = "Media Select";
char NAME_MEDIAMAIL[] = "App Mail";
char NAME_MEDIACALC[] = "App Calculator";
char NAME_MEDIACOMP[] = "App My Computer";
char NAME_WWWSEARCH[] = "WWW Search";
char NAME_WWWHOME[] = "WWW Home";
char NAME_WWWBACK[] = "WWW Back";
char NAME_WWWFORWARD[] = "WWW Forward";
char NAME_WWWSTOP[] = "WWW Stop";
char NAME_WWWREFRESH[] = "WWW Refresh";
char NAME_WWWFAV[] = "WWW Favorites";

char NAME_AMSTRAD_PADEN[] = "Enter (Keypad, Amstrad)";
char NAME_AMSTRAD_RDEL[] = "Forwardspace (Amstrad)";


char LEGEND_A[] = "A";
char LEGEND_B[] = "B";
char LEGEND_C[] = "C";
char LEGEND_D[] = "D";
char LEGEND_E[] = "E";
char LEGEND_F[] = "F";
char LEGEND_G[] = "G";
char LEGEND_H[] = "H";
char LEGEND_I[] = "I";
char LEGEND_J[] = "J";
char LEGEND_K[] = "K";
char LEGEND_L[] = "L";
char LEGEND_M[] = "M";
char LEGEND_N[] = "N";
char LEGEND_O[] = "O";
char LEGEND_P[] = "P";
char LEGEND_Q[] = "Q";
char LEGEND_R[] = "R";
char LEGEND_S[] = "S";
char LEGEND_T[] = "T";
char LEGEND_U[] = "U";
char LEGEND_V[] = "V";
char LEGEND_W[] = "W";
char LEGEND_X[] = "X";
char LEGEND_Y[] = "Y";
char LEGEND_Z[] = "Z";
char LEGEND_0[] = ")\n0";
char LEGEND_1[] = "!\n1";
char LEGEND_2[] = "@\n2";
char LEGEND_3[] = "#\n3";
char LEGEND_4[] = "$\n4";
char LEGEND_5[] = "%\n5";
char LEGEND_6[] = "^\n6";
char LEGEND_7[] = "&\n7";
char LEGEND_8[] = "*\n8";
char LEGEND_9[] = "(\n9)";
char LEGEND_TILDE[] = {0x7E, '\n', 0x60, 0x7C, 0x00};
char LEGEND_DASH[] = "_\n-";
char LEGEND_EQUAL[] = "+\n=";
char LEGEND_BKSLASH[] = "|\\";
char LEGEND_BKSPACE[] = "BKS\n <-";
char LEGEND_SPACE[] = "SPC";
char LEGEND_TAB[] = "TAB";
char LEGEND_CAPS[] = "CAPS\nLOCK";
char LEGEND_LSHIFT[] = "SHI\n^";
char LEGEND_LCTRL[] = "CTL";
char LEGEND_LGUI[] = "GUI";
char LEGEND_LALT[] = "ALT";
char LEGEND_RSHIFT[] = "SHI\n^";
char LEGEND_RCTRL[] = "CTRL";
char LEGEND_RGUI[] = "GUI";
char LEGEND_RALT[] = "ALT";
char LEGEND_APPS[] = "MNU";
char LEGEND_ENTER[] = {'E','N','T','\n', 0x11, 0xC4, 0xD9, 0x00};
char LEGEND_ESC[] = "ESC";
char LEGEND_F1[] = "F1";
char LEGEND_F2[] = "F2";
char LEGEND_F3[] = "F3";
char LEGEND_F4[] = "F4";
char LEGEND_F5[] = "F5";
char LEGEND_F6[] = "F6";
char LEGEND_F7[] = "F7";
char LEGEND_F8[] = "F8";
char LEGEND_F9[] = "F9";
char LEGEND_F10[] = "F10";
char LEGEND_F11[] = "F11";
char LEGEND_F12[] = "F12";
char LEGEND_PRTSC[] = "PRT\nSCR";
char LEGEND_SCROLL[] = "SCR\nLCK";
char LEGEND_PAUSE[] = "PSE\nBRK";
char LEGEND_LEFTSQB[] = "{\n[";
char LEGEND_INSERT[] = "INS";
char LEGEND_HOME[] = "HOM";
char LEGEND_PGUP[] = "PG\nUP";
char LEGEND_DELETE[] = "DEL";
char LEGEND_END[] = "END";
char LEGEND_PGDN[] = "PG\nDN";
char LEGEND_UP[] = {0x1E, 0x00};
char LEGEND_LEFT[] = {0x11, 0x00};
char LEGEND_DOWN[] = {0x1F, 0x00};
char LEGEND_RIGHT[] = {0x10, 0x00};
char LEGEND_NUM[] = "NL";
char LEGEND_PADFWSLASH[] = "/";
char LEGEND_PADASTERISK[] = "*";
char LEGEND_PADMINUS[] = "-";
char LEGEND_PADPLUS[] = "+";
char LEGEND_PADEN[] = "EN\nTR";
char LEGEND_PADDOT[] = ".\nDL";
char LEGEND_PAD0[] = "0\nINS";
char LEGEND_PAD1[] = "1\nEN";
char LEGEND_PAD2[] = {'2', '\n', 0x1F, 0x00};
char LEGEND_PAD3[] = "3\nPD";
char LEGEND_PAD4[] = {'4', '\n', 0x11, 0x00};
char LEGEND_PAD5[] = "5";
char LEGEND_PAD6[] = {'6', '\n', 0x10, 0x00};
char LEGEND_PAD7[] = "7\nHM";
char LEGEND_PAD8[] = {'8', '\n', 0x1E, 0x00};
char LEGEND_PAD9[] = "9\nPU";
char LEGEND_PADCOMMA[] = ",";
char LEGEND_RIGHTSQB[] = "}\n]";
char LEGEND_SEMICOLON[] = ":\n;";
char LEGEND_APOSTROPHE[] = "@\n'";
char LEGEND_COMMA[] = "<\n,";
char LEGEND_PERIOD[] = ">\n.";
char LEGEND_FWSLASH[] = "?\n/";
char LEGEND_EURO1[] = "~\n#";
char LEGEND_EURO2[] = "|\n\\";
char LEGEND_SYSPOWER[] = "PWR";
char LEGEND_SYSSLEEP[] = "SLEP";
char LEGEND_SYSWAKE[] = "WAKE";
char LEGEND_PADEQUAL[] = "=";
char LEGEND_F13[] = "F13";
char LEGEND_F14[] = "F14";
char LEGEND_F15[] = "F15";
char LEGEND_F16[] = "F16";
char LEGEND_F17[] = "F17";
char LEGEND_F18[] = "F18";
char LEGEND_F19[] = "F19";
char LEGEND_F20[] = "F20";
char LEGEND_F21[] = "F21";
char LEGEND_F22[] = "F22";
char LEGEND_F23[] = "F23";
char LEGEND_F24[] = "F24";
char LEGEND_INTL1[] = "Ro";
char LEGEND_INTL2[] = "KAT/HIR";
char LEGEND_INTL3[] = {0x7C, 0x0a, 0x9D, 0x00};
char LEGEND_INTL4[] = "HENKAN";
char LEGEND_INTL5[] = "MUHENKAN";
char LEGEND_INTL6[] = ",";
char LEGEND_LANG1[] = "HAN/YEO";
char LEGEND_LANG2[] = "HANJA";
char LEGEND_LANG3[] = "KATA";
char LEGEND_LANG4[] = "HIRA";
char LEGEND_LANG5[] = "HZK";
char LEGEND_MEDIANEXT[] = {0x10, 0x10, 0x00};
char LEGEND_MEDIAPREV[] = {0x11, 0x11, 0x00};
char LEGEND_MEDIASTOP[] = "S";
char LEGEND_MEDIAPLAY[] = {0x10, 0xDE, 0x00};
char LEGEND_MEDIAMUTE[] = "M";
char LEGEND_MEDIAVOLUP[] = {0x1E, 0x00};
char LEGEND_MEDIAVOLDN[] = {0x1F, 0x00};
char LEGEND_MEDIASEL[] = "SEL";
char LEGEND_MEDIAMAIL[] = "MAIL";
char LEGEND_MEDIACALC[] = "CALC";
char LEGEND_MEDIACOMP[] = "MC";
char LEGEND_WWWSEARCH[] = "SEARCH";
char LEGEND_WWWHOME[] = "HOME";
char LEGEND_WWWBACK[] = {0x11, 0x11, 0x00};
char LEGEND_WWWFORWARD[] = {0x10, 0x10, 0x00};
char LEGEND_WWWSTOP[] = "STOP";
char LEGEND_WWWREFRESH[] = "RFRESH";
char LEGEND_WWWFAV[] = "FAVE";

#define CNCAT2(a,b) a##b
#define CNCAT3(a,b,c) a##b##c

#define DEFINEKEY(keyname,x,y,sx,sy) { CNCAT3(XT_KEY_,keyname,_MAKE), CNCAT3(XT_KEY_,keyname,_BREAK), CNCAT3(KEY_,keyname,_MAKE), CNCAT3(KEY_,keyname,_BREAK), CNCAT3(NAME_,keyname,), CNCAT3(LEGEND_,keyname,),x,y,sx,sy}
#define DEFINEXTKEY(keyname,x,y,sx,sy) { CNCAT3(XT_KEY_,keyname,_MAKE), CNCAT3(XT_KEY_,keyname,_BREAK), NULL, NULL, CNCAT3(NAME_,keyname,), CNCAT3(LEGEND_,keyname,),x,y,sx,sy}
KeyDef KEYDEF_EURO1 = DEFINEKEY(EURO1, 28, 12, 1, 2);

KeyDef BIOSKeyDefs[] = {
    {XT_KEY_BIOS_F11_MAKE, NULL, NULL, NULL, NAME_BIOS_F11,NULL, 46, 1, 3, 1 },
    {XT_KEY_BIOS_F12_MAKE, NULL, NULL, NULL, NAME_BIOS_F12,NULL, 50, 1, 3, 1 }
};



KeyDef KeyDefs[] = {

DEFINEKEY(A, 6, 12, 1, 2),
DEFINEKEY(B, 15, 15, 1, 2 ),
DEFINEKEY(C, 11, 15, 1, 2 ),
DEFINEKEY(D, 10, 12, 1, 2 ),
DEFINEKEY(E, 9, 9, 1, 2 ),
DEFINEKEY(F, 12, 12, 1, 2 ),
DEFINEKEY(G, 14, 12, 1, 2 ),
DEFINEKEY(H, 16, 12, 1, 2 ),
DEFINEKEY(I, 19, 9, 1, 2 ),
DEFINEKEY(J, 18, 12, 1, 2 ),
DEFINEKEY(K, 20, 12, 1, 2 ),
DEFINEKEY(L, 22, 12, 1, 2 ),
DEFINEKEY(M, 19, 15, 1, 2 ),
DEFINEKEY(N, 17, 15, 1, 2 ),
DEFINEKEY(O, 21, 9, 1, 2 ),
DEFINEKEY(P, 23, 9, 1, 2 ),
DEFINEKEY(Q, 5, 9, 1, 2 ),
DEFINEKEY(R, 11, 9, 1, 2 ),
DEFINEKEY(S, 8, 12, 1, 2 ),
DEFINEKEY(T, 13, 9, 1, 2 ),
DEFINEKEY(U, 17, 9, 1, 2 ),
DEFINEKEY(V, 13, 15, 1, 2 ),
DEFINEKEY(W, 7, 9, 1, 2 ),
DEFINEKEY(X, 9, 15, 1, 2 ),
DEFINEKEY(Y, 15, 9, 1, 2 ),
DEFINEKEY(Z, 7, 15, 1, 2 ),
DEFINEKEY(1, 4, 6, 1, 2 ),
DEFINEKEY(2, 6, 6, 1, 2 ),
DEFINEKEY(3, 8, 6, 1, 2 ),
DEFINEKEY(4, 10, 6, 1, 2 ),
DEFINEKEY(5, 12, 6, 1, 2 ),
DEFINEKEY(6, 14, 6, 1, 2 ),
DEFINEKEY(7, 16, 6, 1, 2 ),
DEFINEKEY(8, 18, 6, 1, 2 ),
DEFINEKEY(9, 20, 6, 1, 2 ),
DEFINEKEY(0, 22, 6, 1, 2 ),
DEFINEKEY(TILDE, 1, 6, 2, 2 ),
DEFINEKEY(DASH, 24, 6, 1, 2 ),
DEFINEKEY(EQUAL, 26, 6, 1, 2 ),
DEFINEKEY(BKSLASH, 29, 9, 4, 2 ),
DEFINEKEY(BKSPACE, 30, 6, 3, 2 ),
DEFINEKEY(SPACE, 13, 18, 3, 1 ),
DEFINEKEY(TAB, 1, 9, 3, 2 ),
DEFINEKEY(CAPS, 1, 12, 4, 2 ),
DEFINEKEY(LSHIFT, 1, 15, 3, 2 ),
DEFINEKEY(LCTRL, 1, 18, 3, 1 ),
DEFINEKEY(LGUI, 5, 18, 3, 1 ),
DEFINEKEY(LALT, 9, 18, 3, 1 ),
DEFINEKEY(RSHIFT, 30, 15, 3, 2),
DEFINEKEY(RCTRL, 29, 18, 4, 1 ),
DEFINEKEY(RGUI, 21, 18, 3, 1 ),
DEFINEKEY(RALT, 17, 18, 3, 1 ),
DEFINEKEY(APPS, 25, 18, 3, 1 ),
DEFINEKEY(ENTER, 30, 12, 3, 2 ),
DEFINEKEY(ESC, 1, 1, 3, 1 ),
DEFINEKEY(F1, 6, 3, 3, 1 ),
DEFINEKEY(F2, 10, 3, 3, 1 ),
DEFINEKEY(F3, 14, 3, 3, 1 ),
DEFINEKEY(F4, 18, 3, 3, 1 ),
DEFINEKEY(F5, 23, 3, 3, 1 ),
DEFINEKEY(F6, 27, 3, 3, 1 ),
DEFINEKEY(F7, 31, 3, 3, 1 ),
DEFINEKEY(F8, 35, 3, 3, 1 ),
DEFINEKEY(F9, 40, 3, 3, 1 ),
DEFINEKEY(F10, 44, 3, 3, 1 ),
DEFINEKEY(F11, 48, 3, 3, 1 ),
DEFINEKEY(F12, 52, 3, 3, 1 ),
DEFINEKEY(PRTSC, 35, 6, 3, 2),
DEFINEKEY(SCROLL, 39, 6, 3, 2 ),
{XT_KEY_PAUSE_MAKE, NULL, NULL, NULL, NAME_PAUSE, LEGEND_PAUSE, 43, 6, 3, 2 },
DEFINEKEY(LEFTSQB, 25, 9, 1, 2 ),
DEFINEKEY(INSERT, 35, 9, 3, 2 ),
DEFINEKEY(HOME, 39, 9, 3, 2 ),
DEFINEKEY(PGUP, 43, 9, 3, 2 ),
DEFINEKEY(DELETE, 35, 12, 3, 2 ),
DEFINEKEY(END, 39, 12, 3, 2 ),
DEFINEKEY(PGDN, 43, 12, 3, 2 ),
DEFINEKEY(UP, 39, 16, 3, 1 ),
DEFINEKEY(LEFT, 35, 18, 3, 1 ),
DEFINEKEY(DOWN, 39, 18, 3, 1 ),
DEFINEKEY(RIGHT, 43, 18, 3, 1 ),
DEFINEKEY(NUM, 48, 6, 2, 1 ),
DEFINEKEY(PADFWSLASH, 51, 6, 2, 1 ),
DEFINEKEY(PADASTERISK, 54, 6, 2, 1 ),
DEFINEKEY(PADMINUS, 57, 6, 2, 1 ),
DEFINEKEY(PADPLUS, 57, 8, 2, 2 ),
DEFINEKEY(PADEN, 57, 17, 2, 2 ),
DEFINEKEY(PADDOT, 54, 17, 2, 2 ),
DEFINEKEY(PAD0, 48, 17, 5, 2 ),
DEFINEKEY(PAD1, 48, 14, 2, 2 ),
DEFINEKEY(PAD2, 51, 14, 2, 2 ),
DEFINEKEY(PAD3, 54, 14, 2, 2 ),
DEFINEKEY(PAD4, 48, 11, 2, 2 ),
DEFINEKEY(PAD5, 51, 11, 2, 2 ),
DEFINEKEY(PAD6, 54, 11, 2, 2 ),
DEFINEKEY(PAD7, 48, 8, 2, 2 ),
DEFINEKEY(PAD8, 51, 8, 2, 2 ),
DEFINEKEY(PAD9, 54, 8, 2, 2 ),
DEFINEKEY(RIGHTSQB, 27, 9, 1, 2 ),
DEFINEKEY(SEMICOLON, 24, 12, 1, 2 ),
DEFINEKEY(APOSTROPHE, 26, 12, 1, 2 ),
DEFINEKEY(COMMA, 21, 15, 1, 2 ),
DEFINEKEY(PERIOD, 23, 15, 1, 2 ),
DEFINEKEY(FWSLASH, 25, 15, 1, 2 ),
DEFINEKEY(EURO2, 5, 15, 1, 2 ),
{XT_KEY_BIOS_PADEN_MAKE, NULL, NULL, NULL, NAME_BIOS_PADEN, LEGEND_PADEN, 57, 17, 2, 2 },
{XT_KEY_BIOS_PADFWSLASH_MAKE, NULL, NULL, NULL, NAME_BIOS_PADFWSLASH, LEGEND_PADFWSLASH, 51, 6, 2, 1 },
{XT_KEY_AMSTRAD_PADEN_MAKE, XT_KEY_AMSTRAD_PADEN_BREAK, NULL, NULL, NAME_AMSTRAD_PADEN, LEGEND_PADEN, 57, 17, 2, 2 },
{XT_KEY_AMSTRAD_RDEL_MAKE, XT_KEY_AMSTRAD_RDEL_BREAK, NULL, NULL, NAME_AMSTRAD_RDEL, LEGEND_DELETE, 35, 12, 3, 2 },
DEFINEXTKEY(PADEQUAL, 57, 14, 2, 2 ),
DEFINEXTKEY(PADCOMMA, 57, 11, 2, 2),
DEFINEXTKEY(F13, 6, 1, 3, 1),
DEFINEXTKEY(F14, 10, 1, 3, 1),
DEFINEXTKEY(F15, 14, 1, 3, 1),
DEFINEXTKEY(F16, 18, 1, 3, 1),
DEFINEXTKEY(F17, 23, 1, 3, 1),
DEFINEXTKEY(F18, 27, 1, 3, 1),
DEFINEXTKEY(F19, 31, 1, 3, 1),
DEFINEXTKEY(F20, 35, 1, 3, 1),
DEFINEXTKEY(F21, 40, 1, 3, 1),
DEFINEXTKEY(F22, 44, 1, 3, 1),
DEFINEXTKEY(F23, 48, 1, 3, 1),
DEFINEXTKEY(F24, 52, 1, 3, 1),
DEFINEXTKEY(INTL1, 27, 15, 2, 2),
DEFINEXTKEY(INTL2, 4, 21, 7, 1),
DEFINEXTKEY(INTL3, 28, 6, 1, 2),
DEFINEXTKEY(INTL4, 12, 21, 6, 1),
DEFINEXTKEY(INTL5, 19, 21, 8, 1),
DEFINEXTKEY(INTL6, 28, 21, 1, 1),
DEFINEXTKEY(LANG1, 45, 21, 7, 1),
DEFINEXTKEY(LANG2, 53, 21, 5, 1),
DEFINEXTKEY(LANG3, 30, 21, 4, 1),
DEFINEXTKEY(LANG4, 35, 21, 4, 1),
DEFINEXTKEY(LANG5, 1, 4, 3, 1),
DEFINEXTKEY(MEDIANEXT, 71, 5, 2, 1),
DEFINEXTKEY(MEDIAPREV, 63, 5, 2, 1),
DEFINEXTKEY(MEDIASTOP, 66, 5, 1, 1),
DEFINEXTKEY(MEDIAPLAY, 68, 5, 2, 1),
DEFINEXTKEY(MEDIAMUTE, 78, 5, 1, 1),
DEFINEXTKEY(MEDIAVOLUP, 74, 5, 1, 1),
DEFINEXTKEY(MEDIAVOLDN, 76, 5, 1, 1),
DEFINEXTKEY(MEDIASEL, 63, 7, 3, 1),
DEFINEXTKEY(MEDIAMAIL, 67, 7, 4, 1),
DEFINEXTKEY(MEDIACALC, 72, 7, 4, 1),
DEFINEXTKEY(MEDIACOMP, 77, 7, 2, 1),
DEFINEXTKEY(WWWSEARCH, 66, 10, 6, 1),
DEFINEXTKEY(WWWHOME, 73, 10, 6, 1),
DEFINEXTKEY(WWWBACK, 66, 12, 2, 1),
DEFINEXTKEY(WWWFORWARD, 70, 12, 2, 1),
DEFINEXTKEY(WWWSTOP, 73, 12, 6, 1),
DEFINEXTKEY(WWWREFRESH, 66, 14, 6, 1),
DEFINEXTKEY(WWWFAV, 73, 14, 6, 1),
DEFINEXTKEY(SYSPOWER, 66, 17, 3, 1),
DEFINEXTKEY(SYSSLEEP, 70, 17, 4, 1),
DEFINEXTKEY(SYSWAKE, 75, 17, 4, 1),
};