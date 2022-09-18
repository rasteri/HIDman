#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ps2.h"
#include "defs.h"

__xdata JoyMap JoyPresets[] = {

    {
        0,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Usage Page
        1,                        // Usage
        MAP_KEYBOARD,             // Output Channel
        0x04,                     // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        0,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_X,            // Usage
        MAP_KEYBOARD,              // Output Channel
        0x05,                      // Output Control
        MAP_TYPE_THRESHOLD_ABOVE,  // InputType
        192                        // Input Param
    },
};

__code uint8_t KEY_A_MAKE[] = {1, 0x1C};
__code uint8_t KEY_B_MAKE[] = {1, 0x32};
__code uint8_t KEY_C_MAKE[] = {1, 0x21};
__code uint8_t KEY_D_MAKE[] = {1, 0x23};
__code uint8_t KEY_E_MAKE[] = {1, 0x24};
__code uint8_t KEY_F_MAKE[] = {1, 0x2B};
__code uint8_t KEY_G_MAKE[] = {1, 0x34};
__code uint8_t KEY_H_MAKE[] = {1, 0x33};
__code uint8_t KEY_I_MAKE[] = {1, 0x43};
__code uint8_t KEY_J_MAKE[] = {1, 0x3B};
__code uint8_t KEY_K_MAKE[] = {1, 0x42};
__code uint8_t KEY_L_MAKE[] = {1, 0x4B};
__code uint8_t KEY_M_MAKE[] = {1, 0x3A};
__code uint8_t KEY_N_MAKE[] = {1, 0x31};
__code uint8_t KEY_O_MAKE[] = {1, 0x44};
__code uint8_t KEY_P_MAKE[] = {1, 0x4D};
__code uint8_t KEY_Q_MAKE[] = {1, 0x15};
__code uint8_t KEY_R_MAKE[] = {1, 0x2D};
__code uint8_t KEY_S_MAKE[] = {1, 0x1B};
__code uint8_t KEY_T_MAKE[] = {1, 0x2C};
__code uint8_t KEY_U_MAKE[] = {1, 0x3C};
__code uint8_t KEY_V_MAKE[] = {1, 0x2A};
__code uint8_t KEY_W_MAKE[] = {1, 0x1D};
__code uint8_t KEY_X_MAKE[] = {1, 0x22};
__code uint8_t KEY_Y_MAKE[] = {1, 0x35};
__code uint8_t KEY_Z_MAKE[] = {1, 0x1A};
__code uint8_t KEY_0_MAKE[] = {1, 0x45};
__code uint8_t KEY_1_MAKE[] = {1, 0x16};
__code uint8_t KEY_2_MAKE[] = {1, 0x1E};
__code uint8_t KEY_3_MAKE[] = {1, 0x26};
__code uint8_t KEY_4_MAKE[] = {1, 0x25};
__code uint8_t KEY_5_MAKE[] = {1, 0x2E};
__code uint8_t KEY_6_MAKE[] = {1, 0x36};
__code uint8_t KEY_7_MAKE[] = {1, 0x3D};
__code uint8_t KEY_8_MAKE[] = {1, 0x3E};
__code uint8_t KEY_9_MAKE[] = {1, 0x46};
__code uint8_t KEY_TILDE_MAKE[] = {1, 0x0E};
__code uint8_t KEY_DASH_MAKE[] = {1, 0x4E};
__code uint8_t KEY_EQUAL_MAKE[] = {1, 0x55};
__code uint8_t KEY_BKSLASH_MAKE[] = {1, 0x5D};
__code uint8_t KEY_BKSPACE_MAKE[] = {1, 0x66};
__code uint8_t KEY_SPACE_MAKE[] = {1, 0x29};
__code uint8_t KEY_TAB_MAKE[] = {1, 0x0D};
__code uint8_t KEY_CAPS_MAKE[] = {1, 0x58};
__code uint8_t KEY_LSHIFT_MAKE[] = {1, 0x12};
__code uint8_t KEY_LCTRL_MAKE[] = {1, 0x14};
__code uint8_t KEY_LGUI_MAKE[] = {2, 0xE0, 0x1F};
__code uint8_t KEY_LALT_MAKE[] = {1, 0x11};
__code uint8_t KEY_RSHIFT_MAKE[] = {1, 0x59};
__code uint8_t KEY_RCTRL_MAKE[] = {2, 0xE0, 0x14};
__code uint8_t KEY_RGUI_MAKE[] = {2, 0xE0, 0x27};
__code uint8_t KEY_RALT_MAKE[] = {2, 0xE0, 0x11};
__code uint8_t KEY_APPS_MAKE[] = {2, 0xE0, 0x2F};
__code uint8_t KEY_ENTER_MAKE[] = {1, 0x5A};
__code uint8_t KEY_ESC_MAKE[] = {1, 0x76};
__code uint8_t KEY_F1_MAKE[] = {1, 0x05};
__code uint8_t KEY_F2_MAKE[] = {1, 0x06};
__code uint8_t KEY_F3_MAKE[] = {1, 0x04};
__code uint8_t KEY_F4_MAKE[] = {1, 0x0C};
__code uint8_t KEY_F5_MAKE[] = {1, 0x03};
__code uint8_t KEY_F6_MAKE[] = {1, 0x0B};
__code uint8_t KEY_F7_MAKE[] = {1, 0x83};
__code uint8_t KEY_F8_MAKE[] = {1, 0x0A};
__code uint8_t KEY_F9_MAKE[] = {1, 0x01};
__code uint8_t KEY_F10_MAKE[] = {1, 0x09};
__code uint8_t KEY_F11_MAKE[] = {1, 0x78};
__code uint8_t KEY_F12_MAKE[] = {1, 0x07};
__code uint8_t KEY_PRTSC_MAKE[] = {4, 0xE0, 0x12, 0xE0, 0x7C};
__code uint8_t KEY_SCROLL_MAKE[] = {1, 0x7E};
__code uint8_t KEY_PAUSE_MAKE[] = {8, 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77};
__code uint8_t KEY_LEFTSQB_MAKE[] = {1, 0x54};
__code uint8_t KEY_INSERT_MAKE[] = {2, 0xE0, 0x70};
__code uint8_t KEY_HOME_MAKE[] = {2, 0xE0, 0x6C};
__code uint8_t KEY_PGUP_MAKE[] = {2, 0xE0, 0x7D};
__code uint8_t KEY_DELETE_MAKE[] = {2, 0xE0, 0x71};
__code uint8_t KEY_END_MAKE[] = {2, 0xE0, 0x69};
__code uint8_t KEY_PGDN_MAKE[] = {2, 0xE0, 0x7A};
__code uint8_t KEY_UP_MAKE[] = {2, 0xE0, 0x75};
__code uint8_t KEY_LEFT_MAKE[] = {2, 0xE0, 0x6B};
__code uint8_t KEY_DOWN_MAKE[] = {2, 0xE0, 0x72};
__code uint8_t KEY_RIGHT_MAKE[] = {2, 0xE0, 0x74};
__code uint8_t KEY_NUM_MAKE[] = {1, 0x77};
__code uint8_t KEY_PADFWSLASH_MAKE[] = {2, 0xE0, 0x4A};
__code uint8_t KEY_PADASTERISK_MAKE[] = {1, 0x7C};
__code uint8_t KEY_PADMINUS_MAKE[] = {1, 0x7B};
__code uint8_t KEY_PADPLUS_MAKE[] = {1, 0x79};
__code uint8_t KEY_PADEN_MAKE[] = {2, 0xE0, 0x5A};
__code uint8_t KEY_PADDOT_MAKE[] = {1, 0x71};
__code uint8_t KEY_PAD0_MAKE[] = {1, 0x70};
__code uint8_t KEY_PAD1_MAKE[] = {1, 0x69};
__code uint8_t KEY_PAD2_MAKE[] = {1, 0x71};
__code uint8_t KEY_PAD3_MAKE[] = {1, 0x7A};
__code uint8_t KEY_PAD4_MAKE[] = {1, 0x6B};
__code uint8_t KEY_PAD5_MAKE[] = {1, 0x73};
__code uint8_t KEY_PAD6_MAKE[] = {1, 0x74};
__code uint8_t KEY_PAD7_MAKE[] = {1, 0x6C};
__code uint8_t KEY_PAD8_MAKE[] = {1, 0x75};
__code uint8_t KEY_PAD9_MAKE[] = {1, 0x7D};
__code uint8_t KEY_RIGHTSQB_MAKE[] = {1, 0x5B};
__code uint8_t KEY_SEMICOLON_MAKE[] = {1, 0x4C};
__code uint8_t KEY_APOSTROPHE_MAKE[] = {1, 0x52};
__code uint8_t KEY_COMMA_MAKE[] = {1, 0x41};
__code uint8_t KEY_PERIOD_MAKE[] = {1, 0x49};
__code uint8_t KEY_FWSLASH_MAKE[] = {1, 0x4A};

__code uint8_t KEY_A_BREAK[] = {2, 0xF0, 0x1C};
__code uint8_t KEY_B_BREAK[] = {2, 0xF0, 0x32};
__code uint8_t KEY_C_BREAK[] = {2, 0xF0, 0x21};
__code uint8_t KEY_D_BREAK[] = {2, 0xF0, 0x23};
__code uint8_t KEY_E_BREAK[] = {2, 0xF0, 0x24};
__code uint8_t KEY_F_BREAK[] = {2, 0xF0, 0x2B};
__code uint8_t KEY_G_BREAK[] = {2, 0xF0, 0x34};
__code uint8_t KEY_H_BREAK[] = {2, 0xF0, 0x33};
__code uint8_t KEY_I_BREAK[] = {2, 0xF0, 0x43};
__code uint8_t KEY_J_BREAK[] = {2, 0xF0, 0x3B};
__code uint8_t KEY_K_BREAK[] = {2, 0xF0, 0x42};
__code uint8_t KEY_L_BREAK[] = {2, 0xF0, 0x4B};
__code uint8_t KEY_M_BREAK[] = {2, 0xF0, 0x3A};
__code uint8_t KEY_N_BREAK[] = {2, 0xF0, 0x31};
__code uint8_t KEY_O_BREAK[] = {2, 0xF0, 0x44};
__code uint8_t KEY_P_BREAK[] = {2, 0xF0, 0x4D};
__code uint8_t KEY_Q_BREAK[] = {2, 0xF0, 0x15};
__code uint8_t KEY_R_BREAK[] = {2, 0xF0, 0x2D};
__code uint8_t KEY_S_BREAK[] = {2, 0xF0, 0x1B};
__code uint8_t KEY_T_BREAK[] = {2, 0xF0, 0x2C};
__code uint8_t KEY_U_BREAK[] = {2, 0xF0, 0x3C};
__code uint8_t KEY_V_BREAK[] = {2, 0xF0, 0x2A};
__code uint8_t KEY_W_BREAK[] = {2, 0xF0, 0x1D};
__code uint8_t KEY_X_BREAK[] = {2, 0xF0, 0x22};
__code uint8_t KEY_Y_BREAK[] = {2, 0xF0, 0x35};
__code uint8_t KEY_Z_BREAK[] = {2, 0xF0, 0x1A};
__code uint8_t KEY_0_BREAK[] = {2, 0xF0, 0x45};
__code uint8_t KEY_1_BREAK[] = {2, 0xF0, 0x16};
__code uint8_t KEY_2_BREAK[] = {2, 0xF0, 0x1E};
__code uint8_t KEY_3_BREAK[] = {2, 0xF0, 0x26};
__code uint8_t KEY_4_BREAK[] = {2, 0xF0, 0x25};
__code uint8_t KEY_5_BREAK[] = {2, 0xF0, 0x2E};
__code uint8_t KEY_6_BREAK[] = {2, 0xF0, 0x36};
__code uint8_t KEY_7_BREAK[] = {2, 0xF0, 0x3D};
__code uint8_t KEY_8_BREAK[] = {2, 0xF0, 0x3E};
__code uint8_t KEY_9_BREAK[] = {2, 0xF0, 0x46};
__code uint8_t KEY_TILDE_BREAK[] = {2, 0xF0, 0x0E};
__code uint8_t KEY_DASH_BREAK[] = {2, 0xF0, 0x4E};
__code uint8_t KEY_EQUAL_BREAK[] = {2, 0xF0, 0x55};
__code uint8_t KEY_BKSLASH_BREAK[] = {2, 0xF0, 0x5D};
__code uint8_t KEY_BKSPACE_BREAK[] = {2, 0xF0, 0x66};
__code uint8_t KEY_SPACE_BREAK[] = {2, 0xF0, 0x29};
__code uint8_t KEY_TAB_BREAK[] = {2, 0xF0, 0x0D};
__code uint8_t KEY_CAPS_BREAK[] = {2, 0xF0, 0x58};
__code uint8_t KEY_LSHIFT_BREAK[] = {2, 0xF0, 0x12};
__code uint8_t KEY_LCTRL_BREAK[] = {2, 0xF0, 0x14};
__code uint8_t KEY_LGUI_BREAK[] = {3, 0xE0, 0xF0, 0x1F};
__code uint8_t KEY_LALT_BREAK[] = {2, 0xF0, 0x11};
__code uint8_t KEY_RSHIFT_BREAK[] = {2, 0xF0, 0x59};
__code uint8_t KEY_RCTRL_BREAK[] = {3, 0xE0, 0xF0, 0x14};
__code uint8_t KEY_RGUI_BREAK[] = {3, 0xE0, 0xF0, 0x27};
__code uint8_t KEY_RALT_BREAK[] = {3, 0xE0, 0xF0, 0x11};
__code uint8_t KEY_APPS_BREAK[] = {3, 0xE0, 0xF0, 0x2F};
__code uint8_t KEY_ENTER_BREAK[] = {2, 0xF0, 0x5A};
__code uint8_t KEY_ESC_BREAK[] = {2, 0xF0, 0x76};
__code uint8_t KEY_F1_BREAK[] = {2, 0xF0, 0x05};
__code uint8_t KEY_F2_BREAK[] = {2, 0xF0, 0x06};
__code uint8_t KEY_F3_BREAK[] = {2, 0xF0, 0x04};
__code uint8_t KEY_F4_BREAK[] = {2, 0xF0, 0x0C};
__code uint8_t KEY_F5_BREAK[] = {2, 0xF0, 0x03};
__code uint8_t KEY_F6_BREAK[] = {2, 0xF0, 0x0B};
__code uint8_t KEY_F7_BREAK[] = {2, 0xF0, 0x83};
__code uint8_t KEY_F8_BREAK[] = {2, 0xF0, 0x0A};
__code uint8_t KEY_F9_BREAK[] = {2, 0xF0, 0x01};
__code uint8_t KEY_F10_BREAK[] = {2, 0xF0, 0x09};
__code uint8_t KEY_F11_BREAK[] = {2, 0xF0, 0x78};
__code uint8_t KEY_F12_BREAK[] = {2, 0xF0, 0x07};
__code uint8_t KEY_PRTSC_BREAK[] = {4, 0xE0, 0xF0, 0x7C, 0xE0, 0xF0, 0x12};
__code uint8_t KEY_SCROLL_BREAK[] = {2, 0xF0, 0x7E};
__code uint8_t KEY_PAUSE_BREAK[] = {8, 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77};
__code uint8_t KEY_LEFTSQB_BREAK[] = {2, 0xF0, 0x54};
__code uint8_t KEY_INSERT_BREAK[] = {3, 0xE0, 0xF0, 0x70};
__code uint8_t KEY_HOME_BREAK[] = {3, 0xE0, 0xF0, 0x6C};
__code uint8_t KEY_PGUP_BREAK[] = {3, 0xE0, 0xF0, 0x7D};
__code uint8_t KEY_DELETE_BREAK[] = {3, 0xE0, 0xF0, 0x71};
__code uint8_t KEY_END_BREAK[] = {3, 0xE0, 0xF0, 0x69};
__code uint8_t KEY_PGDN_BREAK[] = {3, 0xE0, 0xF0, 0x7A};
__code uint8_t KEY_UP_BREAK[] = {3, 0xE0, 0xF0, 0x75};
__code uint8_t KEY_LEFT_BREAK[] = {3, 0xE0, 0xF0, 0x6B};
__code uint8_t KEY_DOWN_BREAK[] = {3, 0xE0, 0xF0, 0x72};
__code uint8_t KEY_RIGHT_BREAK[] = {3, 0xE0, 0xF0, 0x74};
__code uint8_t KEY_NUM_BREAK[] = {2, 0xF0, 0x77};
__code uint8_t KEY_PADFWSLASH_BREAK[] = {3, 0xE0, 0xF0, 0x4A};
__code uint8_t KEY_PADASTERISK_BREAK[] = {2, 0xF0, 0x7C};
__code uint8_t KEY_PADMINUS_BREAK[] = {2, 0xF0, 0x7B};
__code uint8_t KEY_PADPLUS_BREAK[] = {2, 0xF0, 0x79};
__code uint8_t KEY_PADEN_BREAK[] = {3, 0xE0, 0xF0, 0x5A};
__code uint8_t KEY_PADDOT_BREAK[] = {2, 0xF0, 0x71};
__code uint8_t KEY_PAD0_BREAK[] = {2, 0xF0, 0x70};
__code uint8_t KEY_PAD1_BREAK[] = {2, 0xF0, 0x69};
__code uint8_t KEY_PAD2_BREAK[] = {2, 0xF0, 0x71};
__code uint8_t KEY_PAD3_BREAK[] = {2, 0xF0, 0x7A};
__code uint8_t KEY_PAD4_BREAK[] = {2, 0xF0, 0x6B};
__code uint8_t KEY_PAD5_BREAK[] = {2, 0xF0, 0x73};
__code uint8_t KEY_PAD6_BREAK[] = {2, 0xF0, 0x74};
__code uint8_t KEY_PAD7_BREAK[] = {2, 0xF0, 0x6C};
__code uint8_t KEY_PAD8_BREAK[] = {2, 0xF0, 0x75};
__code uint8_t KEY_PAD9_BREAK[] = {2, 0xF0, 0x7D};
__code uint8_t KEY_RIGHTSQB_BREAK[] = {2, 0xF0, 0x5B};
__code uint8_t KEY_SEMICOLON_BREAK[] = {2, 0xF0, 0x4C};
__code uint8_t KEY_APOSTROPHE_BREAK[] = {2, 0xF0, 0x52};
__code uint8_t KEY_COMMA_BREAK[] = {2, 0xF0, 0x41};
__code uint8_t KEY_PERIOD_BREAK[] = {2, 0xF0, 0x49};
__code uint8_t KEY_FWSLASH_BREAK[] = {2, 0xF0, 0x4A};

__code uint8_t KEY_ACK[] = {1, 0xFA};
__code uint8_t KEY_BATCOMPLETE[] = {1, 0xAA};
__code uint8_t KEY_ID[] = {2, 0xAB, 0x83};

__code uint8_t KEY_SCANCODE_2[] = {1, 0x02};
__code uint8_t KEY_ECHO[] = {1, 0xEE};
__code uint8_t KEY_ERROR[] = {1, 0xFE};

const uint8_t *const ModtoPS2_MAKE[] =
    {
        KEY_LCTRL_MAKE,
        KEY_LSHIFT_MAKE,
        KEY_LALT_MAKE,
        KEY_LGUI_MAKE,
        KEY_RCTRL_MAKE,
        KEY_RSHIFT_MAKE,
        KEY_RALT_MAKE,
        KEY_RGUI_MAKE};

const uint8_t *const ModtoPS2_BREAK[] =
    {
        KEY_LCTRL_BREAK,
        KEY_LSHIFT_BREAK,
        KEY_LALT_BREAK,
        KEY_LGUI_BREAK,
        KEY_RCTRL_BREAK,
        KEY_RSHIFT_BREAK,
        KEY_RALT_BREAK,
        KEY_RGUI_BREAK};

const uint8_t *const HIDtoPS2_Make[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        KEY_A_MAKE,
        KEY_B_MAKE,
        KEY_C_MAKE,
        KEY_D_MAKE,
        KEY_E_MAKE,
        KEY_F_MAKE,
        KEY_G_MAKE,
        KEY_H_MAKE,
        KEY_I_MAKE,
        KEY_J_MAKE,
        KEY_K_MAKE,
        KEY_L_MAKE,
        KEY_M_MAKE, // 10
        KEY_N_MAKE,
        KEY_O_MAKE,
        KEY_P_MAKE,
        KEY_Q_MAKE,
        KEY_R_MAKE,
        KEY_S_MAKE,
        KEY_T_MAKE,
        KEY_U_MAKE,
        KEY_V_MAKE,
        KEY_W_MAKE,
        KEY_X_MAKE,
        KEY_Y_MAKE,
        KEY_Z_MAKE,
        KEY_1_MAKE,
        KEY_2_MAKE,
        KEY_3_MAKE, //20
        KEY_4_MAKE,
        KEY_5_MAKE,
        KEY_6_MAKE,
        KEY_7_MAKE,
        KEY_8_MAKE,
        KEY_9_MAKE,
        KEY_0_MAKE,
        KEY_ENTER_MAKE,
        KEY_ESC_MAKE,
        KEY_BKSPACE_MAKE,
        KEY_TAB_MAKE,
        KEY_SPACE_MAKE,
        KEY_DASH_MAKE,
        KEY_EQUAL_MAKE,
        KEY_LEFTSQB_MAKE,
        KEY_RIGHTSQB_MAKE, //30
        KEY_BKSLASH_MAKE,
        KEY_TILDE_MAKE,
        KEY_SEMICOLON_MAKE,
        KEY_APOSTROPHE_MAKE,
        NULL,
        KEY_COMMA_MAKE,
        KEY_PERIOD_MAKE,
        KEY_FWSLASH_MAKE,
        KEY_CAPS_MAKE,
        KEY_F1_MAKE,
        KEY_F2_MAKE,
        KEY_F3_MAKE,
        KEY_F4_MAKE,
        KEY_F5_MAKE,
        KEY_F6_MAKE,
        KEY_F7_MAKE, //40
        KEY_F8_MAKE,
        KEY_F9_MAKE,
        KEY_F10_MAKE,
        KEY_F11_MAKE,
        KEY_F12_MAKE,
        KEY_PRTSC_MAKE,
        KEY_SCROLL_MAKE,
        KEY_PAUSE_MAKE,
        KEY_INSERT_MAKE,
        KEY_HOME_MAKE,
        KEY_PGUP_MAKE,
        KEY_DELETE_MAKE,
        KEY_END_MAKE,
        KEY_PGDN_MAKE,
        KEY_RIGHT_MAKE,
        KEY_LEFT_MAKE, // 50
        KEY_DOWN_MAKE,
        KEY_UP_MAKE,
        KEY_NUM_MAKE,
        KEY_PADFWSLASH_MAKE,
        KEY_PADASTERISK_MAKE,
        KEY_PADMINUS_MAKE,
        KEY_PADPLUS_MAKE,
        KEY_PADEN_MAKE,
        KEY_PAD1_MAKE,
        KEY_PAD2_MAKE,
        KEY_PAD3_MAKE,
        KEY_PAD4_MAKE,
        KEY_PAD5_MAKE,
        KEY_PAD6_MAKE,
        KEY_PAD7_MAKE,
        KEY_PAD8_MAKE, // 60
        KEY_PAD9_MAKE,
        KEY_PAD0_MAKE,
        KEY_PADDOT_MAKE,
        NULL,
        KEY_APPS_MAKE,
        NULL,
        KEY_EQUAL_MAKE};

const uint8_t *const HIDtoPS2_Break[] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        KEY_A_BREAK,
        KEY_B_BREAK,
        KEY_C_BREAK,
        KEY_D_BREAK,
        KEY_E_BREAK,
        KEY_F_BREAK,
        KEY_G_BREAK,
        KEY_H_BREAK,
        KEY_I_BREAK,
        KEY_J_BREAK,
        KEY_K_BREAK,
        KEY_L_BREAK,
        KEY_M_BREAK, // 10
        KEY_N_BREAK,
        KEY_O_BREAK,
        KEY_P_BREAK,
        KEY_Q_BREAK,
        KEY_R_BREAK,
        KEY_S_BREAK,
        KEY_T_BREAK,
        KEY_U_BREAK,
        KEY_V_BREAK,
        KEY_W_BREAK,
        KEY_X_BREAK,
        KEY_Y_BREAK,
        KEY_Z_BREAK,
        KEY_1_BREAK,
        KEY_2_BREAK,
        KEY_3_BREAK, //20
        KEY_4_BREAK,
        KEY_5_BREAK,
        KEY_6_BREAK,
        KEY_7_BREAK,
        KEY_8_BREAK,
        KEY_9_BREAK,
        KEY_0_BREAK,
        KEY_ENTER_BREAK,
        KEY_ESC_BREAK,
        KEY_BKSPACE_BREAK,
        KEY_TAB_BREAK,
        KEY_SPACE_BREAK,
        KEY_DASH_BREAK,
        KEY_EQUAL_BREAK,
        KEY_LEFTSQB_BREAK,
        KEY_RIGHTSQB_BREAK, //30
        KEY_BKSLASH_BREAK,
        KEY_TILDE_BREAK,
        KEY_SEMICOLON_BREAK,
        KEY_APOSTROPHE_BREAK,
        NULL,
        KEY_COMMA_BREAK,
        KEY_PERIOD_BREAK,
        KEY_FWSLASH_BREAK,
        KEY_CAPS_BREAK,
        KEY_F1_BREAK,     // = {1, 0x05};
        KEY_F2_BREAK,     // = {1, 0x06};
        KEY_F3_BREAK,     // = {1, 0x04};
        KEY_F4_BREAK,     // = {1, 0x0C};
        KEY_F5_BREAK,     // = {1, 0x03};
        KEY_F6_BREAK,     // = {1, 0x0B};
        KEY_F7_BREAK,     // = {1, 0x83}; 40
        KEY_F8_BREAK,     // = {1, 0x0A};
        KEY_F9_BREAK,     // = {1, 0x01};
        KEY_F10_BREAK,    // = {1, 0x09};
        KEY_F11_BREAK,    // = {1, 0x78};
        KEY_F12_BREAK,    // = {1, 0x07};
        KEY_PRTSC_BREAK,  // = {4, 0xE0, 0x12, 0xE0, 0x7C};
        KEY_SCROLL_BREAK, // = {1, 0x7E};
        KEY_PAUSE_BREAK,  // = {8, 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77};
        KEY_INSERT_BREAK, // = {2, 0xE0, 0x70};
        KEY_HOME_BREAK,   // = {2, 0xE0, 0x6C};
        KEY_PGUP_BREAK,   // = {2, 0xE0, 0x7D};
        KEY_DELETE_BREAK, // = {2, 0xE0, 0x71};
        KEY_END_BREAK,    // = {2, 0xE0, 0x69};
        KEY_PGDN_BREAK,   // = {2, 0xE0, 0x7A};
        KEY_RIGHT_BREAK,
        KEY_LEFT_BREAK, // 50
        KEY_DOWN_BREAK,
        KEY_UP_BREAK,
        KEY_NUM_BREAK,         // = {1, 0x77};
        KEY_PADFWSLASH_BREAK,  // = {2, 0xE0, 0x4A};
        KEY_PADASTERISK_BREAK, // = {1, 0x7C};
        KEY_PADMINUS_BREAK,    // = {1, 0x7B};
        KEY_PADPLUS_BREAK,     // = {1, 0x79};
        KEY_PADEN_BREAK,       // = {2, 0xE0, 0x5A};
        KEY_PAD1_BREAK,        // = {1, 0x69};
        KEY_PAD2_BREAK,        // = {1, 0x71};
        KEY_PAD3_BREAK,        // = {1, 0x7A};
        KEY_PAD4_BREAK,        // = {1, 0x6B};
        KEY_PAD5_BREAK,        // = {1, 0x73};
        KEY_PAD6_BREAK,        // = {1, 0x74};
        KEY_PAD7_BREAK,        // = {1, 0x6C};
        KEY_PAD8_BREAK,        // = {1, 0x75}; 60
        KEY_PAD9_BREAK,        // = {1, 0x7D};
        KEY_PAD0_BREAK,        // = {1, 0x70};
        KEY_PADDOT_BREAK,
        NULL,
        KEY_APPS_BREAK,
        NULL,
        KEY_EQUAL_BREAK};

__code uint8_t ASCIItoHID[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x28, // 10
    0,
    0,
    0x28,
   0,
   0,
   0,
   0,
   0,
   0,
   0, // 20
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0, // 30
   0,
    0x2C,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 40
    0,
    0,
    0x57,
    0,
    0x56,
    0x37 ,
    0,
    0x27,
    0x1e ,
    0x1f , // 50
    0x20 ,
    0x21 ,
    0x22 ,
    0x23 ,
    0x24 ,
    0x25 ,
    0x26 ,
    0 ,
    0,
    0, // 60
    0,
    0,
    0,
    0,
    0x04 ,
    0x05 ,
    0x06 ,
    0x07 ,
    0x08 ,
    0x09 , // 70
    0x0a ,
    0x0b ,
    0x0c ,
    0x0d ,
    0x0e ,
    0x0f ,
    0x10 ,
    0x11 ,
    0x12 ,
    0x13 , // 80
    0x14 ,
    0x15 ,
    0x16 ,
    0x17 ,
    0x18 ,
    0x19 ,
    0x1a ,
    0x1b ,
    0x1c ,
    0x1d , // 90
    0,
    0,
    0,
    0,
    0,
    0,
    0x04,
    0x05,
    0x06,
    0x07, // 100
    0x08,
    0x09, 
    0x0a,
    0x0b,
    0x0c,
    0x0d,
    0x0e,
    0x0f,
    0x10,
    0x11, //110
    0x12,
    0x13, 
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1a,
    0x1b, //120
    0x1c,
    0x1d, 
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0 // 130
};