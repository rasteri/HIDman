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
char NAME_BKSLASH[] = "Backslash/Pipe (USA)";
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
char NAME_EURO1[] = "Hash/Tilde (Europe)";
char NAME_EURO2[] = "Backslash/Pipe (Europe)";

KeyDef KeyDefs[] = {
{ XT_KEY_A_MAKE, XT_KEY_A_BREAK, KEY_A_MAKE, KEY_A_BREAK, NAME_A, 6, 10, 1, 2 },
{ XT_KEY_B_MAKE, XT_KEY_B_BREAK, KEY_B_MAKE, KEY_B_BREAK, NAME_B, 15, 13, 1, 2 },
{ XT_KEY_C_MAKE, XT_KEY_C_BREAK, KEY_C_MAKE, KEY_C_BREAK, NAME_C, 11, 13, 1, 2 },
{ XT_KEY_D_MAKE, XT_KEY_D_BREAK, KEY_D_MAKE, KEY_D_BREAK, NAME_D, 10, 10, 1, 2 },
{ XT_KEY_E_MAKE, XT_KEY_E_BREAK, KEY_E_MAKE, KEY_E_BREAK, NAME_E, 9, 7, 1, 2 },
{ XT_KEY_F_MAKE, XT_KEY_F_BREAK, KEY_F_MAKE, KEY_F_BREAK, NAME_F, 12, 10, 1, 2 },
{ XT_KEY_G_MAKE, XT_KEY_G_BREAK, KEY_G_MAKE, KEY_G_BREAK, NAME_G, 14, 10, 1, 2 },
{ XT_KEY_H_MAKE, XT_KEY_H_BREAK, KEY_H_MAKE, KEY_H_BREAK, NAME_H, 16, 10, 1, 2 },
{ XT_KEY_I_MAKE, XT_KEY_I_BREAK, KEY_I_MAKE, KEY_I_BREAK, NAME_I, 19, 7, 1, 2 },
{ XT_KEY_J_MAKE, XT_KEY_J_BREAK, KEY_J_MAKE, KEY_J_BREAK, NAME_J, 18, 10, 1, 2 },
{ XT_KEY_K_MAKE, XT_KEY_K_BREAK, KEY_K_MAKE, KEY_K_BREAK, NAME_K, 20, 10, 1, 2 },
{ XT_KEY_L_MAKE, XT_KEY_L_BREAK, KEY_L_MAKE, KEY_L_BREAK, NAME_L, 22, 10, 1, 2 },
{ XT_KEY_M_MAKE, XT_KEY_M_BREAK, KEY_M_MAKE, KEY_M_BREAK, NAME_M, 19, 13, 1, 2 },
{ XT_KEY_N_MAKE, XT_KEY_N_BREAK, KEY_N_MAKE, KEY_N_BREAK, NAME_N, 17, 13, 1, 2 },
{ XT_KEY_O_MAKE, XT_KEY_O_BREAK, KEY_O_MAKE, KEY_O_BREAK, NAME_O, 21, 7, 1, 2 },
{ XT_KEY_P_MAKE, XT_KEY_P_BREAK, KEY_P_MAKE, KEY_P_BREAK, NAME_P, 23, 7, 1, 2 },
{ XT_KEY_Q_MAKE, XT_KEY_Q_BREAK, KEY_Q_MAKE, KEY_Q_BREAK, NAME_Q, 5, 7, 1, 2 },
{ XT_KEY_R_MAKE, XT_KEY_R_BREAK, KEY_R_MAKE, KEY_R_BREAK, NAME_R, 11, 7, 1, 2 },
{ XT_KEY_S_MAKE, XT_KEY_S_BREAK, KEY_S_MAKE, KEY_S_BREAK, NAME_S, 8, 10, 1, 2 },
{ XT_KEY_T_MAKE, XT_KEY_T_BREAK, KEY_T_MAKE, KEY_T_BREAK, NAME_T, 13, 7, 1, 2 },
{ XT_KEY_U_MAKE, XT_KEY_U_BREAK, KEY_U_MAKE, KEY_U_BREAK, NAME_U, 17, 7, 1, 2 },
{ XT_KEY_V_MAKE, XT_KEY_V_BREAK, KEY_V_MAKE, KEY_V_BREAK, NAME_V, 13, 13, 1, 2 },
{ XT_KEY_W_MAKE, XT_KEY_W_BREAK, KEY_W_MAKE, KEY_W_BREAK, NAME_W, 7, 7, 1, 2 },
{ XT_KEY_X_MAKE, XT_KEY_X_BREAK, KEY_X_MAKE, KEY_X_BREAK, NAME_X, 9, 13, 1, 2 },
{ XT_KEY_Y_MAKE, XT_KEY_Y_BREAK, KEY_Y_MAKE, KEY_Y_BREAK, NAME_Y, 15, 7, 1, 2 },
{ XT_KEY_Z_MAKE, XT_KEY_Z_BREAK, KEY_Z_MAKE, KEY_Z_BREAK, NAME_Z, 7, 13, 1, 2 },
{ XT_KEY_1_MAKE, XT_KEY_1_BREAK, KEY_1_MAKE, KEY_1_BREAK, NAME_1, 4, 4, 1, 2 },
{ XT_KEY_2_MAKE, XT_KEY_2_BREAK, KEY_2_MAKE, KEY_2_BREAK, NAME_2, 6, 4, 1, 2 },
{ XT_KEY_3_MAKE, XT_KEY_3_BREAK, KEY_3_MAKE, KEY_3_BREAK, NAME_3, 8, 4, 1, 2 },
{ XT_KEY_4_MAKE, XT_KEY_4_BREAK, KEY_4_MAKE, KEY_4_BREAK, NAME_4, 10, 4, 1, 2 },
{ XT_KEY_5_MAKE, XT_KEY_5_BREAK, KEY_5_MAKE, KEY_5_BREAK, NAME_5, 12, 4, 1, 2 },
{ XT_KEY_6_MAKE, XT_KEY_6_BREAK, KEY_6_MAKE, KEY_6_BREAK, NAME_6, 14, 4, 1, 2 },
{ XT_KEY_7_MAKE, XT_KEY_7_BREAK, KEY_7_MAKE, KEY_7_BREAK, NAME_7, 16, 4, 1, 2 },
{ XT_KEY_8_MAKE, XT_KEY_8_BREAK, KEY_8_MAKE, KEY_8_BREAK, NAME_8, 18, 4, 1, 2 },
{ XT_KEY_9_MAKE, XT_KEY_9_BREAK, KEY_9_MAKE, KEY_9_BREAK, NAME_9, 20, 4, 1, 2 },
{ XT_KEY_0_MAKE, XT_KEY_0_BREAK, KEY_0_MAKE, KEY_0_BREAK, NAME_0, 22, 4, 1, 2 },
{ XT_KEY_TILDE_MAKE, XT_KEY_TILDE_BREAK, KEY_TILDE_MAKE, KEY_TILDE_BREAK, NAME_TILDE, 1, 4, 2, 2 },
{ XT_KEY_DASH_MAKE, XT_KEY_DASH_BREAK, KEY_DASH_MAKE, KEY_DASH_BREAK, NAME_DASH, 24, 4, 1, 2 },
{ XT_KEY_EQUAL_MAKE, XT_KEY_EQUAL_BREAK, KEY_EQUAL_MAKE, KEY_EQUAL_BREAK, NAME_EQUAL, 26, 4, 1, 2 },
{ XT_KEY_BKSPACE_MAKE, XT_KEY_BKSPACE_BREAK, KEY_BKSPACE_MAKE, KEY_BKSPACE_BREAK, NAME_BKSPACE, 28, 4, 5, 2 },
{ XT_KEY_SPACE_MAKE, XT_KEY_SPACE_BREAK, KEY_SPACE_MAKE, KEY_SPACE_BREAK, NAME_SPACE, 13, 16, 3, 1 },
{ XT_KEY_TAB_MAKE, XT_KEY_TAB_BREAK, KEY_TAB_MAKE, KEY_TAB_BREAK, NAME_TAB, 1, 7, 3, 2 },
};