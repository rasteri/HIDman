#include<stdio.h>
#ifndef __SDCC
#define __code
#else
#include"defs.h"
#endif
__code unsigned char KEY_SET1_PWR_MAKE[] = { 0x02, 0xE0, 0x5E, };
__code unsigned char KEY_SET1_PWR_BREAK[] = { 0x02, 0xE0, 0xDE, };
__code unsigned char KEY_SET2_PWR_MAKE[] = { 0x02, 0xE0, 0x37, };
__code unsigned char KEY_SET2_PWR_BREAK[] = { 0x03, 0xE0, 0xF0, 0x37, };

__code unsigned char KEY_SET1_SLEP_MAKE[] = { 0x02, 0xE0, 0x5F, };
__code unsigned char KEY_SET1_SLEP_BREAK[] = { 0x02, 0xE0, 0xDF, };
__code unsigned char KEY_SET2_SLEP_MAKE[] = { 0x02, 0xE0, 0x3F, };
__code unsigned char KEY_SET2_SLEP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x3F, };

__code unsigned char KEY_SET1_WAKE_MAKE[] = { 0x02, 0xE0, 0x63, };
__code unsigned char KEY_SET1_WAKE_BREAK[] = { 0x02, 0xE0, 0xE3, };
__code unsigned char KEY_SET2_WAKE_MAKE[] = { 0x02, 0xE0, 0x5E, };
__code unsigned char KEY_SET2_WAKE_BREAK[] = { 0x03, 0xE0, 0xF0, 0x5E, };

__code unsigned char KEY_SET1_A_MAKE[] = { 0x01, 0x1E, };
__code unsigned char KEY_SET1_A_BREAK[] = { 0x01, 0x9E, };
__code unsigned char KEY_SET2_A_MAKE[] = { 0x01, 0x1C, };
__code unsigned char KEY_SET2_A_BREAK[] = { 0x02, 0xF0, 0x1C, };

__code unsigned char KEY_SET1_B_MAKE[] = { 0x01, 0x30, };
__code unsigned char KEY_SET1_B_BREAK[] = { 0x01, 0xB0, };
__code unsigned char KEY_SET2_B_MAKE[] = { 0x01, 0x32, };
__code unsigned char KEY_SET2_B_BREAK[] = { 0x02, 0xF0, 0x32, };

__code unsigned char KEY_SET1_C_MAKE[] = { 0x01, 0x2E, };
__code unsigned char KEY_SET1_C_BREAK[] = { 0x01, 0xAE, };
__code unsigned char KEY_SET2_C_MAKE[] = { 0x01, 0x21, };
__code unsigned char KEY_SET2_C_BREAK[] = { 0x02, 0xF0, 0x21, };

__code unsigned char KEY_SET1_D_MAKE[] = { 0x01, 0x20, };
__code unsigned char KEY_SET1_D_BREAK[] = { 0x01, 0xA0, };
__code unsigned char KEY_SET2_D_MAKE[] = { 0x01, 0x23, };
__code unsigned char KEY_SET2_D_BREAK[] = { 0x02, 0xF0, 0x23, };

__code unsigned char KEY_SET1_E_MAKE[] = { 0x01, 0x12, };
__code unsigned char KEY_SET1_E_BREAK[] = { 0x01, 0x92, };
__code unsigned char KEY_SET2_E_MAKE[] = { 0x01, 0x24, };
__code unsigned char KEY_SET2_E_BREAK[] = { 0x02, 0xF0, 0x24, };

__code unsigned char KEY_SET1_F_MAKE[] = { 0x01, 0x21, };
__code unsigned char KEY_SET1_F_BREAK[] = { 0x01, 0xA1, };
__code unsigned char KEY_SET2_F_MAKE[] = { 0x01, 0x2B, };
__code unsigned char KEY_SET2_F_BREAK[] = { 0x02, 0xF0, 0x2B, };

__code unsigned char KEY_SET1_G_MAKE[] = { 0x01, 0x22, };
__code unsigned char KEY_SET1_G_BREAK[] = { 0x01, 0xA2, };
__code unsigned char KEY_SET2_G_MAKE[] = { 0x01, 0x34, };
__code unsigned char KEY_SET2_G_BREAK[] = { 0x02, 0xF0, 0x34, };

__code unsigned char KEY_SET1_H_MAKE[] = { 0x01, 0x23, };
__code unsigned char KEY_SET1_H_BREAK[] = { 0x01, 0xA3, };
__code unsigned char KEY_SET2_H_MAKE[] = { 0x01, 0x33, };
__code unsigned char KEY_SET2_H_BREAK[] = { 0x02, 0xF0, 0x33, };

__code unsigned char KEY_SET1_I_MAKE[] = { 0x01, 0x17, };
__code unsigned char KEY_SET1_I_BREAK[] = { 0x01, 0x97, };
__code unsigned char KEY_SET2_I_MAKE[] = { 0x01, 0x43, };
__code unsigned char KEY_SET2_I_BREAK[] = { 0x02, 0xF0, 0x43, };

__code unsigned char KEY_SET1_J_MAKE[] = { 0x01, 0x24, };
__code unsigned char KEY_SET1_J_BREAK[] = { 0x01, 0xA4, };
__code unsigned char KEY_SET2_J_MAKE[] = { 0x01, 0x3B, };
__code unsigned char KEY_SET2_J_BREAK[] = { 0x02, 0xF0, 0x3B, };

__code unsigned char KEY_SET1_K_MAKE[] = { 0x01, 0x25, };
__code unsigned char KEY_SET1_K_BREAK[] = { 0x01, 0xA5, };
__code unsigned char KEY_SET2_K_MAKE[] = { 0x01, 0x42, };
__code unsigned char KEY_SET2_K_BREAK[] = { 0x02, 0xF0, 0x42, };

__code unsigned char KEY_SET1_L_MAKE[] = { 0x01, 0x26, };
__code unsigned char KEY_SET1_L_BREAK[] = { 0x01, 0xA6, };
__code unsigned char KEY_SET2_L_MAKE[] = { 0x01, 0x4B, };
__code unsigned char KEY_SET2_L_BREAK[] = { 0x02, 0xF0, 0x4B, };

__code unsigned char KEY_SET1_M_MAKE[] = { 0x01, 0x32, };
__code unsigned char KEY_SET1_M_BREAK[] = { 0x01, 0xB2, };
__code unsigned char KEY_SET2_M_MAKE[] = { 0x01, 0x3A, };
__code unsigned char KEY_SET2_M_BREAK[] = { 0x02, 0xF0, 0x3A, };

__code unsigned char KEY_SET1_N_MAKE[] = { 0x01, 0x31, };
__code unsigned char KEY_SET1_N_BREAK[] = { 0x01, 0xB1, };
__code unsigned char KEY_SET2_N_MAKE[] = { 0x01, 0x31, };
__code unsigned char KEY_SET2_N_BREAK[] = { 0x02, 0xF0, 0x31, };

__code unsigned char KEY_SET1_O_MAKE[] = { 0x01, 0x18, };
__code unsigned char KEY_SET1_O_BREAK[] = { 0x01, 0x98, };
__code unsigned char KEY_SET2_O_MAKE[] = { 0x01, 0x44, };
__code unsigned char KEY_SET2_O_BREAK[] = { 0x02, 0xF0, 0x44, };

__code unsigned char KEY_SET1_P_MAKE[] = { 0x01, 0x19, };
__code unsigned char KEY_SET1_P_BREAK[] = { 0x01, 0x99, };
__code unsigned char KEY_SET2_P_MAKE[] = { 0x01, 0x4D, };
__code unsigned char KEY_SET2_P_BREAK[] = { 0x02, 0xF0, 0x4D, };

__code unsigned char KEY_SET1_Q_MAKE[] = { 0x01, 0x10, };
__code unsigned char KEY_SET1_Q_BREAK[] = { 0x01, 0x90, };
__code unsigned char KEY_SET2_Q_MAKE[] = { 0x01, 0x15, };
__code unsigned char KEY_SET2_Q_BREAK[] = { 0x02, 0xF0, 0x15, };

__code unsigned char KEY_SET1_R_MAKE[] = { 0x01, 0x13, };
__code unsigned char KEY_SET1_R_BREAK[] = { 0x01, 0x93, };
__code unsigned char KEY_SET2_R_MAKE[] = { 0x01, 0x2D, };
__code unsigned char KEY_SET2_R_BREAK[] = { 0x02, 0xF0, 0x2D, };

__code unsigned char KEY_SET1_S_MAKE[] = { 0x01, 0x1F, };
__code unsigned char KEY_SET1_S_BREAK[] = { 0x01, 0x9F, };
__code unsigned char KEY_SET2_S_MAKE[] = { 0x01, 0x1B, };
__code unsigned char KEY_SET2_S_BREAK[] = { 0x02, 0xF0, 0x1B, };

__code unsigned char KEY_SET1_T_MAKE[] = { 0x01, 0x14, };
__code unsigned char KEY_SET1_T_BREAK[] = { 0x01, 0x94, };
__code unsigned char KEY_SET2_T_MAKE[] = { 0x01, 0x2C, };
__code unsigned char KEY_SET2_T_BREAK[] = { 0x02, 0xF0, 0x2C, };

__code unsigned char KEY_SET1_U_MAKE[] = { 0x01, 0x16, };
__code unsigned char KEY_SET1_U_BREAK[] = { 0x01, 0x96, };
__code unsigned char KEY_SET2_U_MAKE[] = { 0x01, 0x3C, };
__code unsigned char KEY_SET2_U_BREAK[] = { 0x02, 0xF0, 0x3C, };

__code unsigned char KEY_SET1_V_MAKE[] = { 0x01, 0x2F, };
__code unsigned char KEY_SET1_V_BREAK[] = { 0x01, 0xAF, };
__code unsigned char KEY_SET2_V_MAKE[] = { 0x01, 0x2A, };
__code unsigned char KEY_SET2_V_BREAK[] = { 0x02, 0xF0, 0x2A, };

__code unsigned char KEY_SET1_W_MAKE[] = { 0x01, 0x11, };
__code unsigned char KEY_SET1_W_BREAK[] = { 0x01, 0x91, };
__code unsigned char KEY_SET2_W_MAKE[] = { 0x01, 0x1D, };
__code unsigned char KEY_SET2_W_BREAK[] = { 0x02, 0xF0, 0x1D, };

__code unsigned char KEY_SET1_X_MAKE[] = { 0x01, 0x2D, };
__code unsigned char KEY_SET1_X_BREAK[] = { 0x01, 0xAD, };
__code unsigned char KEY_SET2_X_MAKE[] = { 0x01, 0x22, };
__code unsigned char KEY_SET2_X_BREAK[] = { 0x02, 0xF0, 0x22, };

__code unsigned char KEY_SET1_Y_MAKE[] = { 0x01, 0x15, };
__code unsigned char KEY_SET1_Y_BREAK[] = { 0x01, 0x95, };
__code unsigned char KEY_SET2_Y_MAKE[] = { 0x01, 0x35, };
__code unsigned char KEY_SET2_Y_BREAK[] = { 0x02, 0xF0, 0x35, };

__code unsigned char KEY_SET1_Z_MAKE[] = { 0x01, 0x2C, };
__code unsigned char KEY_SET1_Z_BREAK[] = { 0x01, 0xAC, };
__code unsigned char KEY_SET2_Z_MAKE[] = { 0x01, 0x1A, };
__code unsigned char KEY_SET2_Z_BREAK[] = { 0x02, 0xF0, 0x1A, };

__code unsigned char KEY_SET1_1_MAKE[] = { 0x01, 0x02, };
__code unsigned char KEY_SET1_1_BREAK[] = { 0x01, 0x82, };
__code unsigned char KEY_SET2_1_MAKE[] = { 0x01, 0x16, };
__code unsigned char KEY_SET2_1_BREAK[] = { 0x02, 0xF0, 0x16, };

__code unsigned char KEY_SET1_2_MAKE[] = { 0x01, 0x03, };
__code unsigned char KEY_SET1_2_BREAK[] = { 0x01, 0x83, };
__code unsigned char KEY_SET2_2_MAKE[] = { 0x01, 0x1E, };
__code unsigned char KEY_SET2_2_BREAK[] = { 0x02, 0xF0, 0x1E, };

__code unsigned char KEY_SET1_3_MAKE[] = { 0x01, 0x04, };
__code unsigned char KEY_SET1_3_BREAK[] = { 0x01, 0x84, };
__code unsigned char KEY_SET2_3_MAKE[] = { 0x01, 0x26, };
__code unsigned char KEY_SET2_3_BREAK[] = { 0x02, 0xF0, 0x26, };

__code unsigned char KEY_SET1_4_MAKE[] = { 0x01, 0x05, };
__code unsigned char KEY_SET1_4_BREAK[] = { 0x01, 0x85, };
__code unsigned char KEY_SET2_4_MAKE[] = { 0x01, 0x25, };
__code unsigned char KEY_SET2_4_BREAK[] = { 0x02, 0xF0, 0x25, };

__code unsigned char KEY_SET1_5_MAKE[] = { 0x01, 0x06, };
__code unsigned char KEY_SET1_5_BREAK[] = { 0x01, 0x86, };
__code unsigned char KEY_SET2_5_MAKE[] = { 0x01, 0x2E, };
__code unsigned char KEY_SET2_5_BREAK[] = { 0x02, 0xF0, 0x2E, };

__code unsigned char KEY_SET1_6_MAKE[] = { 0x01, 0x07, };
__code unsigned char KEY_SET1_6_BREAK[] = { 0x01, 0x87, };
__code unsigned char KEY_SET2_6_MAKE[] = { 0x01, 0x36, };
__code unsigned char KEY_SET2_6_BREAK[] = { 0x02, 0xF0, 0x36, };

__code unsigned char KEY_SET1_7_MAKE[] = { 0x01, 0x08, };
__code unsigned char KEY_SET1_7_BREAK[] = { 0x01, 0x88, };
__code unsigned char KEY_SET2_7_MAKE[] = { 0x01, 0x3D, };
__code unsigned char KEY_SET2_7_BREAK[] = { 0x02, 0xF0, 0x3D, };

__code unsigned char KEY_SET1_8_MAKE[] = { 0x01, 0x09, };
__code unsigned char KEY_SET1_8_BREAK[] = { 0x01, 0x89, };
__code unsigned char KEY_SET2_8_MAKE[] = { 0x01, 0x3E, };
__code unsigned char KEY_SET2_8_BREAK[] = { 0x02, 0xF0, 0x3E, };

__code unsigned char KEY_SET1_9_MAKE[] = { 0x01, 0x0A, };
__code unsigned char KEY_SET1_9_BREAK[] = { 0x01, 0x8A, };
__code unsigned char KEY_SET2_9_MAKE[] = { 0x01, 0x46, };
__code unsigned char KEY_SET2_9_BREAK[] = { 0x02, 0xF0, 0x46, };

__code unsigned char KEY_SET1_0_MAKE[] = { 0x01, 0x0B, };
__code unsigned char KEY_SET1_0_BREAK[] = { 0x01, 0x8B, };
__code unsigned char KEY_SET2_0_MAKE[] = { 0x01, 0x45, };
__code unsigned char KEY_SET2_0_BREAK[] = { 0x02, 0xF0, 0x45, };

__code unsigned char KEY_SET1_ENTER_MAKE[] = { 0x01, 0x1C, };
__code unsigned char KEY_SET1_ENTER_BREAK[] = { 0x01, 0x9C, };
__code unsigned char KEY_SET2_ENTER_MAKE[] = { 0x01, 0x5A, };
__code unsigned char KEY_SET2_ENTER_BREAK[] = { 0x02, 0xF0, 0x5A, };

__code unsigned char KEY_SET1_ESCAPE_MAKE[] = { 0x01, 0x01, };
__code unsigned char KEY_SET1_ESCAPE_BREAK[] = { 0x01, 0x81, };
__code unsigned char KEY_SET2_ESCAPE_MAKE[] = { 0x01, 0x76, };
__code unsigned char KEY_SET2_ESCAPE_BREAK[] = { 0x02, 0xF0, 0x76, };

__code unsigned char KEY_SET1_BKSPACE_MAKE[] = { 0x01, 0x0E, };
__code unsigned char KEY_SET1_BKSPACE_BREAK[] = { 0x01, 0x8E, };
__code unsigned char KEY_SET2_BKSPACE_MAKE[] = { 0x01, 0x66, };
__code unsigned char KEY_SET2_BKSPACE_BREAK[] = { 0x02, 0xF0, 0x66, };

__code unsigned char KEY_SET1_TAB_MAKE[] = { 0x01, 0x0F, };
__code unsigned char KEY_SET1_TAB_BREAK[] = { 0x01, 0x8F, };
__code unsigned char KEY_SET2_TAB_MAKE[] = { 0x01, 0x0D, };
__code unsigned char KEY_SET2_TAB_BREAK[] = { 0x02, 0xF0, 0x0D, };

__code unsigned char KEY_SET1_SPACE_MAKE[] = { 0x01, 0x39, };
__code unsigned char KEY_SET1_SPACE_BREAK[] = { 0x01, 0xB9, };
__code unsigned char KEY_SET2_SPACE_MAKE[] = { 0x01, 0x29, };
__code unsigned char KEY_SET2_SPACE_BREAK[] = { 0x02, 0xF0, 0x29, };

__code unsigned char KEY_SET1_DASH_MAKE[] = { 0x01, 0x0C, };
__code unsigned char KEY_SET1_DASH_BREAK[] = { 0x01, 0x8C, };
__code unsigned char KEY_SET2_DASH_MAKE[] = { 0x01, 0x4E, };
__code unsigned char KEY_SET2_DASH_BREAK[] = { 0x02, 0xF0, 0x4E, };

__code unsigned char KEY_SET1_EQUALS_MAKE[] = { 0x01, 0x0D, };
__code unsigned char KEY_SET1_EQUALS_BREAK[] = { 0x01, 0x8D, };
__code unsigned char KEY_SET2_EQUALS_MAKE[] = { 0x01, 0x55, };
__code unsigned char KEY_SET2_EQUALS_BREAK[] = { 0x02, 0xF0, 0x55, };

__code unsigned char KEY_SET1_LEFTSQB_MAKE[] = { 0x01, 0x1A, };
__code unsigned char KEY_SET1_LEFTSQB_BREAK[] = { 0x01, 0x9A, };
__code unsigned char KEY_SET2_LEFTSQB_MAKE[] = { 0x01, 0x54, };
__code unsigned char KEY_SET2_LEFTSQB_BREAK[] = { 0x02, 0xF0, 0x54, };

__code unsigned char KEY_SET1_RIGHTSQB_MAKE[] = { 0x01, 0x1B, };
__code unsigned char KEY_SET1_RIGHTSQB_BREAK[] = { 0x01, 0x9B, };
__code unsigned char KEY_SET2_RIGHTSQB_MAKE[] = { 0x01, 0x5B, };
__code unsigned char KEY_SET2_RIGHTSQB_BREAK[] = { 0x02, 0xF0, 0x5B, };

__code unsigned char KEY_SET1_BKSLASH_MAKE[] = { 0x01, 0x2B, };
__code unsigned char KEY_SET1_BKSLASH_BREAK[] = { 0x01, 0xAB, };
__code unsigned char KEY_SET2_BKSLASH_MAKE[] = { 0x01, 0x5D, };
__code unsigned char KEY_SET2_BKSLASH_BREAK[] = { 0x02, 0xF0, 0x5D, };

__code unsigned char KEY_SET1_EURO1_MAKE[] = { 0x01, 0x2B, };
__code unsigned char KEY_SET1_EURO1_BREAK[] = { 0x01, 0xAB, };
__code unsigned char KEY_SET2_EURO1_MAKE[] = { 0x01, 0x5D, };
__code unsigned char KEY_SET2_EURO1_BREAK[] = { 0x02, 0xF0, 0x5D, };

__code unsigned char KEY_SET1_SEMICOLON_MAKE[] = { 0x01, 0x27, };
__code unsigned char KEY_SET1_SEMICOLON_BREAK[] = { 0x01, 0xA7, };
__code unsigned char KEY_SET2_SEMICOLON_MAKE[] = { 0x01, 0x4C, };
__code unsigned char KEY_SET2_SEMICOLON_BREAK[] = { 0x02, 0xF0, 0x4C, };

__code unsigned char KEY_SET1_APOSTROPHE_MAKE[] = { 0x01, 0x28, };
__code unsigned char KEY_SET1_APOSTROPHE_BREAK[] = { 0x01, 0xA8, };
__code unsigned char KEY_SET2_APOSTROPHE_MAKE[] = { 0x01, 0x52, };
__code unsigned char KEY_SET2_APOSTROPHE_BREAK[] = { 0x02, 0xF0, 0x52, };

__code unsigned char KEY_SET1_TILDE_MAKE[] = { 0x01, 0x29, };
__code unsigned char KEY_SET1_TILDE_BREAK[] = { 0x01, 0xA9, };
__code unsigned char KEY_SET2_TILDE_MAKE[] = { 0x01, 0x0E, };
__code unsigned char KEY_SET2_TILDE_BREAK[] = { 0x02, 0xF0, 0x0E, };

__code unsigned char KEY_SET1_COMMA_MAKE[] = { 0x01, 0x33, };
__code unsigned char KEY_SET1_COMMA_BREAK[] = { 0x01, 0xB3, };
__code unsigned char KEY_SET2_COMMA_MAKE[] = { 0x01, 0x41, };
__code unsigned char KEY_SET2_COMMA_BREAK[] = { 0x02, 0xF0, 0x41, };

__code unsigned char KEY_SET1_PERIOD_MAKE[] = { 0x01, 0x34, };
__code unsigned char KEY_SET1_PERIOD_BREAK[] = { 0x01, 0xB4, };
__code unsigned char KEY_SET2_PERIOD_MAKE[] = { 0x01, 0x49, };
__code unsigned char KEY_SET2_PERIOD_BREAK[] = { 0x02, 0xF0, 0x49, };

__code unsigned char KEY_SET1_FWSLASH_MAKE[] = { 0x01, 0x35, };
__code unsigned char KEY_SET1_FWSLASH_BREAK[] = { 0x01, 0xB5, };
__code unsigned char KEY_SET2_FWSLASH_MAKE[] = { 0x01, 0x4A, };
__code unsigned char KEY_SET2_FWSLASH_BREAK[] = { 0x02, 0xF0, 0x4A, };

__code unsigned char KEY_SET1_CAPS_MAKE[] = { 0x01, 0x3A, };
__code unsigned char KEY_SET1_CAPS_BREAK[] = { 0x01, 0xBA, };
__code unsigned char KEY_SET2_CAPS_MAKE[] = { 0x01, 0x58, };
__code unsigned char KEY_SET2_CAPS_BREAK[] = { 0x02, 0xF0, 0x58, };

__code unsigned char KEY_SET1_F1_MAKE[] = { 0x01, 0x3B, };
__code unsigned char KEY_SET1_F1_BREAK[] = { 0x01, 0xBB, };
__code unsigned char KEY_SET2_F1_MAKE[] = { 0x01, 0x05, };
__code unsigned char KEY_SET2_F1_BREAK[] = { 0x02, 0xF0, 0x05, };

__code unsigned char KEY_SET1_F2_MAKE[] = { 0x01, 0x3C, };
__code unsigned char KEY_SET1_F2_BREAK[] = { 0x01, 0xBC, };
__code unsigned char KEY_SET2_F2_MAKE[] = { 0x01, 0x06, };
__code unsigned char KEY_SET2_F2_BREAK[] = { 0x02, 0xF0, 0x06, };

__code unsigned char KEY_SET1_F3_MAKE[] = { 0x01, 0x3D, };
__code unsigned char KEY_SET1_F3_BREAK[] = { 0x01, 0xBD, };
__code unsigned char KEY_SET2_F3_MAKE[] = { 0x01, 0x04, };
__code unsigned char KEY_SET2_F3_BREAK[] = { 0x02, 0xF0, 0x04, };

__code unsigned char KEY_SET1_F4_MAKE[] = { 0x01, 0x3E, };
__code unsigned char KEY_SET1_F4_BREAK[] = { 0x01, 0xBE, };
__code unsigned char KEY_SET2_F4_MAKE[] = { 0x01, 0x0C, };
__code unsigned char KEY_SET2_F4_BREAK[] = { 0x02, 0xF0, 0x0C, };

__code unsigned char KEY_SET1_F5_MAKE[] = { 0x01, 0x3F, };
__code unsigned char KEY_SET1_F5_BREAK[] = { 0x01, 0xBF, };
__code unsigned char KEY_SET2_F5_MAKE[] = { 0x01, 0x03, };
__code unsigned char KEY_SET2_F5_BREAK[] = { 0x02, 0xF0, 0x03, };

__code unsigned char KEY_SET1_F6_MAKE[] = { 0x01, 0x40, };
__code unsigned char KEY_SET1_F6_BREAK[] = { 0x01, 0xC0, };
__code unsigned char KEY_SET2_F6_MAKE[] = { 0x01, 0x0B, };
__code unsigned char KEY_SET2_F6_BREAK[] = { 0x02, 0xF0, 0x0B, };

__code unsigned char KEY_SET1_F7_MAKE[] = { 0x01, 0x41, };
__code unsigned char KEY_SET1_F7_BREAK[] = { 0x01, 0xC1, };
__code unsigned char KEY_SET2_F7_MAKE[] = { 0x01, 0x83, };
__code unsigned char KEY_SET2_F7_BREAK[] = { 0x02, 0xF0, 0x83, };

__code unsigned char KEY_SET1_F8_MAKE[] = { 0x01, 0x42, };
__code unsigned char KEY_SET1_F8_BREAK[] = { 0x01, 0xC2, };
__code unsigned char KEY_SET2_F8_MAKE[] = { 0x01, 0x0A, };
__code unsigned char KEY_SET2_F8_BREAK[] = { 0x02, 0xF0, 0x0A, };

__code unsigned char KEY_SET1_F9_MAKE[] = { 0x01, 0x43, };
__code unsigned char KEY_SET1_F9_BREAK[] = { 0x01, 0xC3, };
__code unsigned char KEY_SET2_F9_MAKE[] = { 0x01, 0x01, };
__code unsigned char KEY_SET2_F9_BREAK[] = { 0x02, 0xF0, 0x01, };

__code unsigned char KEY_SET1_F10_MAKE[] = { 0x01, 0x44, };
__code unsigned char KEY_SET1_F10_BREAK[] = { 0x01, 0xC4, };
__code unsigned char KEY_SET2_F10_MAKE[] = { 0x01, 0x09, };
__code unsigned char KEY_SET2_F10_BREAK[] = { 0x02, 0xF0, 0x09, };

__code unsigned char KEY_SET1_F11_MAKE[] = { 0x01, 0x57, };
__code unsigned char KEY_SET1_F11_BREAK[] = { 0x01, 0xD7, };
__code unsigned char KEY_SET2_F11_MAKE[] = { 0x01, 0x78, };
__code unsigned char KEY_SET2_F11_BREAK[] = { 0x02, 0xF0, 0x78, };

__code unsigned char KEY_SET1_F12_MAKE[] = { 0x01, 0x58, };
__code unsigned char KEY_SET1_F12_BREAK[] = { 0x01, 0xD8, };
__code unsigned char KEY_SET2_F12_MAKE[] = { 0x01, 0x07, };
__code unsigned char KEY_SET2_F12_BREAK[] = { 0x02, 0xF0, 0x07, };

__code unsigned char KEY_SET1_PRTSCR_MAKE[] = { 0x02, 0xE0, 0x37, };
__code unsigned char KEY_SET1_PRTSCR_BREAK[] = { 0x02, 0xE0, 0xB7, };
__code unsigned char KEY_SET2_PRTSCR_MAKE[] = { 0x02, 0xE0, 0x7C, };
__code unsigned char KEY_SET2_PRTSCR_BREAK[] = { 0x03, 0xE0, 0xF0, 0x7C, };

__code unsigned char KEY_SET1_SCRLOCK_MAKE[] = { 0x01, 0x46, };
__code unsigned char KEY_SET1_SCRLOCK_BREAK[] = { 0x01, 0xC6, };
__code unsigned char KEY_SET2_SCRLOCK_MAKE[] = { 0x01, 0x7E, };
__code unsigned char KEY_SET2_SCRLOCK_BREAK[] = { 0x02, 0xF0, 0x7E, };

__code unsigned char KEY_SET1_PAUSE_MAKE[] = { 0x06, 0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5, };
__code unsigned char KEY_SET2_PAUSE_MAKE[] = { 0x08, 0xE1, 0x14, 0x77, 0xE1, 0xF0, 0x14, 0xF0, 0x77, };
__code unsigned char KEY_SET1_INSERT_MAKE[] = { 0x02, 0xE0, 0x52, };
__code unsigned char KEY_SET1_INSERT_BREAK[] = { 0x02, 0xE0, 0xD2, };
__code unsigned char KEY_SET2_INSERT_MAKE[] = { 0x02, 0xE0, 0x70, };
__code unsigned char KEY_SET2_INSERT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x70, };

__code unsigned char KEY_SET1_HOME_MAKE[] = { 0x02, 0xE0, 0x47, };
__code unsigned char KEY_SET1_HOME_BREAK[] = { 0x02, 0xE0, 0xC7, };
__code unsigned char KEY_SET2_HOME_MAKE[] = { 0x02, 0xE0, 0x6C, };
__code unsigned char KEY_SET2_HOME_BREAK[] = { 0x03, 0xE0, 0xF0, 0x6C, };

__code unsigned char KEY_SET1_PGUP_MAKE[] = { 0x02, 0xE0, 0x49, };
__code unsigned char KEY_SET1_PGUP_BREAK[] = { 0x02, 0xE0, 0xC9, };
__code unsigned char KEY_SET2_PGUP_MAKE[] = { 0x02, 0xE0, 0x7D, };
__code unsigned char KEY_SET2_PGUP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x7D, };

__code unsigned char KEY_SET1_DELETE_MAKE[] = { 0x02, 0xE0, 0x53, };
__code unsigned char KEY_SET1_DELETE_BREAK[] = { 0x02, 0xE0, 0xD3, };
__code unsigned char KEY_SET2_DELETE_MAKE[] = { 0x02, 0xE0, 0x71, };
__code unsigned char KEY_SET2_DELETE_BREAK[] = { 0x03, 0xE0, 0xF0, 0x71, };

__code unsigned char KEY_SET1_END_MAKE[] = { 0x02, 0xE0, 0x4F, };
__code unsigned char KEY_SET1_END_BREAK[] = { 0x02, 0xE0, 0xCF, };
__code unsigned char KEY_SET2_END_MAKE[] = { 0x02, 0xE0, 0x69, };
__code unsigned char KEY_SET2_END_BREAK[] = { 0x03, 0xE0, 0xF0, 0x69, };

__code unsigned char KEY_SET1_PGDN_MAKE[] = { 0x02, 0xE0, 0x51, };
__code unsigned char KEY_SET1_PGDN_BREAK[] = { 0x02, 0xE0, 0xD1, };
__code unsigned char KEY_SET2_PGDN_MAKE[] = { 0x02, 0xE0, 0x7A, };
__code unsigned char KEY_SET2_PGDN_BREAK[] = { 0x03, 0xE0, 0xF0, 0x7A, };

__code unsigned char KEY_SET1_RIGHT_MAKE[] = { 0x02, 0xE0, 0x4D, };
__code unsigned char KEY_SET1_RIGHT_BREAK[] = { 0x02, 0xE0, 0xCD, };
__code unsigned char KEY_SET2_RIGHT_MAKE[] = { 0x02, 0xE0, 0x74, };
__code unsigned char KEY_SET2_RIGHT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x74, };

__code unsigned char KEY_SET1_LEFT_MAKE[] = { 0x02, 0xE0, 0x4B, };
__code unsigned char KEY_SET1_LEFT_BREAK[] = { 0x02, 0xE0, 0xCB, };
__code unsigned char KEY_SET2_LEFT_MAKE[] = { 0x02, 0xE0, 0x6B, };
__code unsigned char KEY_SET2_LEFT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x6B, };

__code unsigned char KEY_SET1_DOWN_MAKE[] = { 0x02, 0xE0, 0x50, };
__code unsigned char KEY_SET1_DOWN_BREAK[] = { 0x02, 0xE0, 0xD0, };
__code unsigned char KEY_SET2_DOWN_MAKE[] = { 0x02, 0xE0, 0x72, };
__code unsigned char KEY_SET2_DOWN_BREAK[] = { 0x03, 0xE0, 0xF0, 0x72, };

__code unsigned char KEY_SET1_UP_MAKE[] = { 0x02, 0xE0, 0x48, };
__code unsigned char KEY_SET1_UP_BREAK[] = { 0x02, 0xE0, 0xC8, };
__code unsigned char KEY_SET2_UP_MAKE[] = { 0x02, 0xE0, 0x75, };
__code unsigned char KEY_SET2_UP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x75, };

__code unsigned char KEY_SET1_NUMLCK_MAKE[] = { 0x01, 0x45, };
__code unsigned char KEY_SET1_NUMLCK_BREAK[] = { 0x01, 0xC5, };
__code unsigned char KEY_SET2_NUMLCK_MAKE[] = { 0x01, 0x77, };
__code unsigned char KEY_SET2_NUMLCK_BREAK[] = { 0x02, 0xF0, 0x77, };

__code unsigned char KEY_SET1_PADFWSLASH_MAKE[] = { 0x02, 0xE0, 0x35, };
__code unsigned char KEY_SET1_PADFWSLASH_BREAK[] = { 0x02, 0xE0, 0xB5, };
__code unsigned char KEY_SET2_PADFWSLASH_MAKE[] = { 0x02, 0xE0, 0x4A, };
__code unsigned char KEY_SET2_PADFWSLASH_BREAK[] = { 0x03, 0xE0, 0xF0, 0x4A, };

__code unsigned char KEY_SET1_PADASTERISK_MAKE[] = { 0x01, 0x37, };
__code unsigned char KEY_SET1_PADASTERISK_BREAK[] = { 0x01, 0xB7, };
__code unsigned char KEY_SET2_PADASTERISK_MAKE[] = { 0x01, 0x7C, };
__code unsigned char KEY_SET2_PADASTERISK_BREAK[] = { 0x02, 0xF0, 0x7C, };

__code unsigned char KEY_SET1_PADMINUS_MAKE[] = { 0x01, 0x4A, };
__code unsigned char KEY_SET1_PADMINUS_BREAK[] = { 0x01, 0xCA, };
__code unsigned char KEY_SET2_PADMINUS_MAKE[] = { 0x01, 0x7B, };
__code unsigned char KEY_SET2_PADMINUS_BREAK[] = { 0x02, 0xF0, 0x7B, };

__code unsigned char KEY_SET1_PADPLUS_MAKE[] = { 0x01, 0x4E, };
__code unsigned char KEY_SET1_PADPLUS_BREAK[] = { 0x01, 0xCE, };
__code unsigned char KEY_SET2_PADPLUS_MAKE[] = { 0x01, 0x79, };
__code unsigned char KEY_SET2_PADPLUS_BREAK[] = { 0x02, 0xF0, 0x79, };

__code unsigned char KEY_SET1_PADENTER_MAKE[] = { 0x02, 0xE0, 0x1C, };
__code unsigned char KEY_SET1_PADENTER_BREAK[] = { 0x02, 0xE0, 0x9C, };
__code unsigned char KEY_SET2_PADENTER_MAKE[] = { 0x02, 0xE0, 0x5A, };
__code unsigned char KEY_SET2_PADENTER_BREAK[] = { 0x03, 0xE0, 0xF0, 0x5A, };

__code unsigned char KEY_SET1_PAD1_MAKE[] = { 0x01, 0x4F, };
__code unsigned char KEY_SET1_PAD1_BREAK[] = { 0x01, 0xCF, };
__code unsigned char KEY_SET2_PAD1_MAKE[] = { 0x01, 0x69, };
__code unsigned char KEY_SET2_PAD1_BREAK[] = { 0x02, 0xF0, 0x69, };

__code unsigned char KEY_SET1_PAD2_MAKE[] = { 0x01, 0x50, };
__code unsigned char KEY_SET1_PAD2_BREAK[] = { 0x01, 0xD0, };
__code unsigned char KEY_SET2_PAD2_MAKE[] = { 0x01, 0x72, };
__code unsigned char KEY_SET2_PAD2_BREAK[] = { 0x02, 0xF0, 0x72, };

__code unsigned char KEY_SET1_PAD3_MAKE[] = { 0x01, 0x51, };
__code unsigned char KEY_SET1_PAD3_BREAK[] = { 0x01, 0xD1, };
__code unsigned char KEY_SET2_PAD3_MAKE[] = { 0x01, 0x7A, };
__code unsigned char KEY_SET2_PAD3_BREAK[] = { 0x02, 0xF0, 0x7A, };

__code unsigned char KEY_SET1_PAD4_MAKE[] = { 0x01, 0x4B, };
__code unsigned char KEY_SET1_PAD4_BREAK[] = { 0x01, 0xCB, };
__code unsigned char KEY_SET2_PAD4_MAKE[] = { 0x01, 0x6B, };
__code unsigned char KEY_SET2_PAD4_BREAK[] = { 0x02, 0xF0, 0x6B, };

__code unsigned char KEY_SET1_PAD5_MAKE[] = { 0x01, 0x4C, };
__code unsigned char KEY_SET1_PAD5_BREAK[] = { 0x01, 0xCC, };
__code unsigned char KEY_SET2_PAD5_MAKE[] = { 0x01, 0x73, };
__code unsigned char KEY_SET2_PAD5_BREAK[] = { 0x02, 0xF0, 0x73, };

__code unsigned char KEY_SET1_PAD6_MAKE[] = { 0x01, 0x4D, };
__code unsigned char KEY_SET1_PAD6_BREAK[] = { 0x01, 0xCD, };
__code unsigned char KEY_SET2_PAD6_MAKE[] = { 0x01, 0x74, };
__code unsigned char KEY_SET2_PAD6_BREAK[] = { 0x02, 0xF0, 0x74, };

__code unsigned char KEY_SET1_PAD7_MAKE[] = { 0x01, 0x47, };
__code unsigned char KEY_SET1_PAD7_BREAK[] = { 0x01, 0xC7, };
__code unsigned char KEY_SET2_PAD7_MAKE[] = { 0x01, 0x6C, };
__code unsigned char KEY_SET2_PAD7_BREAK[] = { 0x02, 0xF0, 0x6C, };

__code unsigned char KEY_SET1_PAD8_MAKE[] = { 0x01, 0x48, };
__code unsigned char KEY_SET1_PAD8_BREAK[] = { 0x01, 0xC8, };
__code unsigned char KEY_SET2_PAD8_MAKE[] = { 0x01, 0x75, };
__code unsigned char KEY_SET2_PAD8_BREAK[] = { 0x02, 0xF0, 0x75, };

__code unsigned char KEY_SET1_PAD9_MAKE[] = { 0x01, 0x49, };
__code unsigned char KEY_SET1_PAD9_BREAK[] = { 0x01, 0xC9, };
__code unsigned char KEY_SET2_PAD9_MAKE[] = { 0x01, 0x7D, };
__code unsigned char KEY_SET2_PAD9_BREAK[] = { 0x02, 0xF0, 0x7D, };

__code unsigned char KEY_SET1_PAD0_MAKE[] = { 0x01, 0x52, };
__code unsigned char KEY_SET1_PAD0_BREAK[] = { 0x01, 0xD2, };
__code unsigned char KEY_SET2_PAD0_MAKE[] = { 0x01, 0x70, };
__code unsigned char KEY_SET2_PAD0_BREAK[] = { 0x02, 0xF0, 0x70, };

__code unsigned char KEY_SET1_PADPERIOD_MAKE[] = { 0x01, 0x53, };
__code unsigned char KEY_SET1_PADPERIOD_BREAK[] = { 0x01, 0xD3, };
__code unsigned char KEY_SET2_PADPERIOD_MAKE[] = { 0x01, 0x71, };
__code unsigned char KEY_SET2_PADPERIOD_BREAK[] = { 0x02, 0xF0, 0x71, };

__code unsigned char KEY_SET1_EURO2_MAKE[] = { 0x01, 0x56, };
__code unsigned char KEY_SET1_EURO2_BREAK[] = { 0x01, 0xD6, };
__code unsigned char KEY_SET2_EURO2_MAKE[] = { 0x01, 0x61, };
__code unsigned char KEY_SET2_EURO2_BREAK[] = { 0x02, 0xF0, 0x61, };

__code unsigned char KEY_SET1_APP_MAKE[] = { 0x02, 0xE0, 0x5D, };
__code unsigned char KEY_SET1_APP_BREAK[] = { 0x02, 0xE0, 0xDD, };
__code unsigned char KEY_SET2_APP_MAKE[] = { 0x02, 0xE0, 0x2F, };
__code unsigned char KEY_SET2_APP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x2F, };

__code unsigned char KEY_SET1_KEYPOWER_MAKE[] = { 0x02, 0xE0, 0x5E, };
__code unsigned char KEY_SET1_KEYPOWER_BREAK[] = { 0x02, 0xE0, 0xDE, };
__code unsigned char KEY_SET2_KEYPOWER_MAKE[] = { 0x02, 0xE0, 0x37, };
__code unsigned char KEY_SET2_KEYPOWER_BREAK[] = { 0x03, 0xE0, 0xF0, 0x37, };

__code unsigned char KEY_SET1_PADEQUALS_MAKE[] = { 0x01, 0x59, };
__code unsigned char KEY_SET1_PADEQUALS_BREAK[] = { 0x01, 0xD9, };
__code unsigned char KEY_SET2_PADEQUALS_MAKE[] = { 0x01, 0x0F, };
__code unsigned char KEY_SET2_PADEQUALS_BREAK[] = { 0x02, 0xF0, 0x0F, };

__code unsigned char KEY_SET1_F13_MAKE[] = { 0x01, 0x64, };
__code unsigned char KEY_SET1_F13_BREAK[] = { 0x01, 0xE4, };
__code unsigned char KEY_SET2_F13_MAKE[] = { 0x01, 0x08, };
__code unsigned char KEY_SET2_F13_BREAK[] = { 0x02, 0xF0, 0x08, };

__code unsigned char KEY_SET1_F14_MAKE[] = { 0x01, 0x65, };
__code unsigned char KEY_SET1_F14_BREAK[] = { 0x01, 0xE5, };
__code unsigned char KEY_SET2_F14_MAKE[] = { 0x01, 0x10, };
__code unsigned char KEY_SET2_F14_BREAK[] = { 0x02, 0xF0, 0x10, };

__code unsigned char KEY_SET1_F15_MAKE[] = { 0x01, 0x66, };
__code unsigned char KEY_SET1_F15_BREAK[] = { 0x01, 0xE6, };
__code unsigned char KEY_SET2_F15_MAKE[] = { 0x01, 0x18, };
__code unsigned char KEY_SET2_F15_BREAK[] = { 0x02, 0xF0, 0x18, };

__code unsigned char KEY_SET1_F16_MAKE[] = { 0x01, 0x67, };
__code unsigned char KEY_SET1_F16_BREAK[] = { 0x01, 0xE7, };
__code unsigned char KEY_SET2_F16_MAKE[] = { 0x01, 0x20, };
__code unsigned char KEY_SET2_F16_BREAK[] = { 0x02, 0xF0, 0x20, };

__code unsigned char KEY_SET1_F17_MAKE[] = { 0x01, 0x68, };
__code unsigned char KEY_SET1_F17_BREAK[] = { 0x01, 0xE8, };
__code unsigned char KEY_SET2_F17_MAKE[] = { 0x01, 0x28, };
__code unsigned char KEY_SET2_F17_BREAK[] = { 0x02, 0xF0, 0x28, };

__code unsigned char KEY_SET1_F18_MAKE[] = { 0x01, 0x69, };
__code unsigned char KEY_SET1_F18_BREAK[] = { 0x01, 0xE9, };
__code unsigned char KEY_SET2_F18_MAKE[] = { 0x01, 0x30, };
__code unsigned char KEY_SET2_F18_BREAK[] = { 0x02, 0xF0, 0x30, };

__code unsigned char KEY_SET1_F19_MAKE[] = { 0x01, 0x6A, };
__code unsigned char KEY_SET1_F19_BREAK[] = { 0x01, 0xEA, };
__code unsigned char KEY_SET2_F19_MAKE[] = { 0x01, 0x38, };
__code unsigned char KEY_SET2_F19_BREAK[] = { 0x02, 0xF0, 0x38, };

__code unsigned char KEY_SET1_F20_MAKE[] = { 0x01, 0x6B, };
__code unsigned char KEY_SET1_F20_BREAK[] = { 0x01, 0xEB, };
__code unsigned char KEY_SET2_F20_MAKE[] = { 0x01, 0x40, };
__code unsigned char KEY_SET2_F20_BREAK[] = { 0x02, 0xF0, 0x40, };

__code unsigned char KEY_SET1_F21_MAKE[] = { 0x01, 0x6C, };
__code unsigned char KEY_SET1_F21_BREAK[] = { 0x01, 0xEC, };
__code unsigned char KEY_SET2_F21_MAKE[] = { 0x01, 0x48, };
__code unsigned char KEY_SET2_F21_BREAK[] = { 0x02, 0xF0, 0x48, };

__code unsigned char KEY_SET1_F22_MAKE[] = { 0x01, 0x6D, };
__code unsigned char KEY_SET1_F22_BREAK[] = { 0x01, 0xED, };
__code unsigned char KEY_SET2_F22_MAKE[] = { 0x01, 0x50, };
__code unsigned char KEY_SET2_F22_BREAK[] = { 0x02, 0xF0, 0x50, };

__code unsigned char KEY_SET1_F23_MAKE[] = { 0x01, 0x6E, };
__code unsigned char KEY_SET1_F23_BREAK[] = { 0x01, 0xEE, };
__code unsigned char KEY_SET2_F23_MAKE[] = { 0x01, 0x57, };
__code unsigned char KEY_SET2_F23_BREAK[] = { 0x02, 0xF0, 0x57, };

__code unsigned char KEY_SET1_F24_MAKE[] = { 0x01, 0x76, };
__code unsigned char KEY_SET1_F24_BREAK[] = { 0x01, 0xF6, };
__code unsigned char KEY_SET2_F24_MAKE[] = { 0x01, 0x5F, };
__code unsigned char KEY_SET2_F24_BREAK[] = { 0x02, 0xF0, 0x5F, };

__code unsigned char KEY_SET1_PADCOMMA_MAKE[] = { 0x01, 0x7E, };
__code unsigned char KEY_SET1_PADCOMMA_BREAK[] = { 0x01, 0xFE, };
__code unsigned char KEY_SET2_PADCOMMA_MAKE[] = { 0x01, 0x6D, };
__code unsigned char KEY_SET2_PADCOMMA_BREAK[] = { 0x02, 0xF0, 0x6D, };

__code unsigned char KEY_SET1_INTL1_MAKE[] = { 0x01, 0x73, };
__code unsigned char KEY_SET1_INTL1_BREAK[] = { 0x01, 0xF3, };
__code unsigned char KEY_SET2_INTL1_MAKE[] = { 0x01, 0x51, };
__code unsigned char KEY_SET2_INTL1_BREAK[] = { 0x02, 0xF0, 0x51, };

__code unsigned char KEY_SET1_INTL2_MAKE[] = { 0x01, 0x70, };
__code unsigned char KEY_SET1_INTL2_BREAK[] = { 0x01, 0xF0, };
__code unsigned char KEY_SET2_INTL2_MAKE[] = { 0x01, 0x13, };
__code unsigned char KEY_SET2_INTL2_BREAK[] = { 0x02, 0xF0, 0x13, };

__code unsigned char KEY_SET1_INTL3_MAKE[] = { 0x01, 0x7D, };
__code unsigned char KEY_SET1_INTL3_BREAK[] = { 0x01, 0xFD, };
__code unsigned char KEY_SET2_INTL3_MAKE[] = { 0x01, 0x6A, };
__code unsigned char KEY_SET2_INTL3_BREAK[] = { 0x02, 0xF0, 0x6A, };

__code unsigned char KEY_SET1_INTL4_MAKE[] = { 0x01, 0x79, };
__code unsigned char KEY_SET1_INTL4_BREAK[] = { 0x01, 0xF9, };
__code unsigned char KEY_SET2_INTL4_MAKE[] = { 0x01, 0x64, };
__code unsigned char KEY_SET2_INTL4_BREAK[] = { 0x02, 0xF0, 0x64, };

__code unsigned char KEY_SET1_INTL5_MAKE[] = { 0x01, 0x7B, };
__code unsigned char KEY_SET1_INTL5_BREAK[] = { 0x01, 0xFB, };
__code unsigned char KEY_SET2_INTL5_MAKE[] = { 0x01, 0x67, };
__code unsigned char KEY_SET2_INTL5_BREAK[] = { 0x02, 0xF0, 0x67, };

__code unsigned char KEY_SET1_INTL6_MAKE[] = { 0x01, 0x5C, };
__code unsigned char KEY_SET1_INTL6_BREAK[] = { 0x01, 0xDC, };
__code unsigned char KEY_SET2_INTL6_MAKE[] = { 0x01, 0x27, };
__code unsigned char KEY_SET2_INTL6_BREAK[] = { 0x02, 0xF0, 0x27, };

__code unsigned char KEY_SET1_LANG1_MAKE[] = { 0x01, 0xF2, };
__code unsigned char KEY_SET2_LANG1_MAKE[] = { 0x01, 0xF2, };
__code unsigned char KEY_SET1_LANG2_MAKE[] = { 0x01, 0xF1, };
__code unsigned char KEY_SET2_LANG2_MAKE[] = { 0x01, 0xF1, };
__code unsigned char KEY_SET1_LANG3_MAKE[] = { 0x01, 0x78, };
__code unsigned char KEY_SET1_LANG3_BREAK[] = { 0x01, 0xF8, };
__code unsigned char KEY_SET2_LANG3_MAKE[] = { 0x01, 0x63, };
__code unsigned char KEY_SET2_LANG3_BREAK[] = { 0x02, 0xF0, 0x63, };

__code unsigned char KEY_SET1_LANG4_MAKE[] = { 0x01, 0x77, };
__code unsigned char KEY_SET1_LANG4_BREAK[] = { 0x01, 0xF7, };
__code unsigned char KEY_SET2_LANG4_MAKE[] = { 0x01, 0x62, };
__code unsigned char KEY_SET2_LANG4_BREAK[] = { 0x02, 0xF0, 0x62, };

__code unsigned char KEY_SET1_LANG5_MAKE[] = { 0x01, 0x76, };
__code unsigned char KEY_SET1_LANG5_BREAK[] = { 0x01, 0xF6, };
__code unsigned char KEY_SET2_LANG5_MAKE[] = { 0x01, 0x5F, };
__code unsigned char KEY_SET2_LANG5_BREAK[] = { 0x02, 0xF0, 0x5F, };

__code unsigned char KEY_SET1_LCTRL_MAKE[] = { 0x01, 0x1D, };
__code unsigned char KEY_SET1_LCTRL_BREAK[] = { 0x01, 0x9D, };
__code unsigned char KEY_SET2_LCTRL_MAKE[] = { 0x01, 0x14, };
__code unsigned char KEY_SET2_LCTRL_BREAK[] = { 0x02, 0xF0, 0x14, };

__code unsigned char KEY_SET1_LSHIFT_MAKE[] = { 0x01, 0x2A, };
__code unsigned char KEY_SET1_LSHIFT_BREAK[] = { 0x01, 0xAA, };
__code unsigned char KEY_SET2_LSHIFT_MAKE[] = { 0x01, 0x12, };
__code unsigned char KEY_SET2_LSHIFT_BREAK[] = { 0x02, 0xF0, 0x12, };

__code unsigned char KEY_SET1_LALT_MAKE[] = { 0x01, 0x38, };
__code unsigned char KEY_SET1_LALT_BREAK[] = { 0x01, 0xB8, };
__code unsigned char KEY_SET2_LALT_MAKE[] = { 0x01, 0x11, };
__code unsigned char KEY_SET2_LALT_BREAK[] = { 0x02, 0xF0, 0x11, };

__code unsigned char KEY_SET1_LGUI_MAKE[] = { 0x02, 0xE0, 0x5B, };
__code unsigned char KEY_SET1_LGUI_BREAK[] = { 0x02, 0xE0, 0xDB, };
__code unsigned char KEY_SET2_LGUI_MAKE[] = { 0x02, 0xE0, 0x1F, };
__code unsigned char KEY_SET2_LGUI_BREAK[] = { 0x03, 0xE0, 0xF0, 0x1F, };

__code unsigned char KEY_SET1_RCTRL_MAKE[] = { 0x02, 0xE0, 0x1D, };
__code unsigned char KEY_SET1_RCTRL_BREAK[] = { 0x02, 0xE0, 0x9D, };
__code unsigned char KEY_SET2_RCTRL_MAKE[] = { 0x02, 0xE0, 0x14, };
__code unsigned char KEY_SET2_RCTRL_BREAK[] = { 0x03, 0xE0, 0xF0, 0x14, };

__code unsigned char KEY_SET1_RSHIFT_MAKE[] = { 0x01, 0x36, };
__code unsigned char KEY_SET1_RSHIFT_BREAK[] = { 0x01, 0xB6, };
__code unsigned char KEY_SET2_RSHIFT_MAKE[] = { 0x01, 0x59, };
__code unsigned char KEY_SET2_RSHIFT_BREAK[] = { 0x02, 0xF0, 0x59, };

__code unsigned char KEY_SET1_RALT_MAKE[] = { 0x02, 0xE0, 0x38, };
__code unsigned char KEY_SET1_RALT_BREAK[] = { 0x02, 0xE0, 0xB8, };
__code unsigned char KEY_SET2_RALT_MAKE[] = { 0x02, 0xE0, 0x11, };
__code unsigned char KEY_SET2_RALT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x11, };

__code unsigned char KEY_SET1_RGUI_MAKE[] = { 0x02, 0xE0, 0x5C, };
__code unsigned char KEY_SET1_RGUI_BREAK[] = { 0x02, 0xE0, 0xDC, };
__code unsigned char KEY_SET2_RGUI_MAKE[] = { 0x02, 0xE0, 0x27, };
__code unsigned char KEY_SET2_RGUI_BREAK[] = { 0x03, 0xE0, 0xF0, 0x27, };

__code unsigned char KEY_SET1_MEDIANEXT_MAKE[] = { 0x02, 0xE0, 0x19, };
__code unsigned char KEY_SET1_MEDIANEXT_BREAK[] = { 0x02, 0xE0, 0x99, };
__code unsigned char KEY_SET2_MEDIANEXT_MAKE[] = { 0x02, 0xE0, 0x4D, };
__code unsigned char KEY_SET2_MEDIANEXT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x4D, };

__code unsigned char KEY_SET1_MEDIAPREV_MAKE[] = { 0x02, 0xE0, 0x10, };
__code unsigned char KEY_SET1_MEDIAPREV_BREAK[] = { 0x02, 0xE0, 0x90, };
__code unsigned char KEY_SET2_MEDIAPREV_MAKE[] = { 0x02, 0xE0, 0x15, };
__code unsigned char KEY_SET2_MEDIAPREV_BREAK[] = { 0x03, 0xE0, 0xF0, 0x15, };

__code unsigned char KEY_SET1_MEDIASTOP_MAKE[] = { 0x02, 0xE0, 0x24, };
__code unsigned char KEY_SET1_MEDIASTOP_BREAK[] = { 0x02, 0xE0, 0xA4, };
__code unsigned char KEY_SET2_MEDIASTOP_MAKE[] = { 0x02, 0xE0, 0x3B, };
__code unsigned char KEY_SET2_MEDIASTOP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x3B, };

__code unsigned char KEY_SET1_MEDIANPLAY_MAKE[] = { 0x02, 0xE0, 0x22, };
__code unsigned char KEY_SET1_MEDIANPLAY_BREAK[] = { 0x02, 0xE0, 0xA2, };
__code unsigned char KEY_SET2_MEDIANPLAY_MAKE[] = { 0x02, 0xE0, 0x34, };
__code unsigned char KEY_SET2_MEDIANPLAY_BREAK[] = { 0x03, 0xE0, 0xF0, 0x34, };

__code unsigned char KEY_SET1_MEDIAMUTE_MAKE[] = { 0x02, 0xE0, 0x20, };
__code unsigned char KEY_SET1_MEDIAMUTE_BREAK[] = { 0x02, 0xE0, 0xA0, };
__code unsigned char KEY_SET2_MEDIAMUTE_MAKE[] = { 0x02, 0xE0, 0x23, };
__code unsigned char KEY_SET2_MEDIAMUTE_BREAK[] = { 0x03, 0xE0, 0xF0, 0x23, };

__code unsigned char KEY_SET1_VOLUP_MAKE[] = { 0x02, 0xE0, 0x30, };
__code unsigned char KEY_SET1_VOLUP_BREAK[] = { 0x02, 0xE0, 0xB0, };
__code unsigned char KEY_SET2_VOLUP_MAKE[] = { 0x02, 0xE0, 0x32, };
__code unsigned char KEY_SET2_VOLUP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x32, };

__code unsigned char KEY_SET1_VOLDN_MAKE[] = { 0x02, 0xE0, 0x2E, };
__code unsigned char KEY_SET1_VOLDN_BREAK[] = { 0x02, 0xE0, 0xAE, };
__code unsigned char KEY_SET2_VOLDN_MAKE[] = { 0x02, 0xE0, 0x21, };
__code unsigned char KEY_SET2_VOLDN_BREAK[] = { 0x03, 0xE0, 0xF0, 0x21, };

__code unsigned char KEY_SET1_MEDSELECT_MAKE[] = { 0x02, 0xE0, 0x6D, };
__code unsigned char KEY_SET1_MEDSELECT_BREAK[] = { 0x02, 0xE0, 0xED, };
__code unsigned char KEY_SET2_MEDSELECT_MAKE[] = { 0x02, 0xE0, 0x50, };
__code unsigned char KEY_SET2_MEDSELECT_BREAK[] = { 0x03, 0xE0, 0xF0, 0x50, };

__code unsigned char KEY_SET1_APPMAIL_MAKE[] = { 0x02, 0xE0, 0x6C, };
__code unsigned char KEY_SET1_APPMAIL_BREAK[] = { 0x02, 0xE0, 0xEC, };
__code unsigned char KEY_SET2_APPMAIL_MAKE[] = { 0x02, 0xE0, 0x48, };
__code unsigned char KEY_SET2_APPMAIL_BREAK[] = { 0x03, 0xE0, 0xF0, 0x48, };

__code unsigned char KEY_SET1_APPCALC_MAKE[] = { 0x02, 0xE0, 0x21, };
__code unsigned char KEY_SET1_APPCALC_BREAK[] = { 0x02, 0xE0, 0xA1, };
__code unsigned char KEY_SET2_APPCALC_MAKE[] = { 0x02, 0xE0, 0x2B, };
__code unsigned char KEY_SET2_APPCALC_BREAK[] = { 0x03, 0xE0, 0xF0, 0x2B, };

__code unsigned char KEY_SET1_APPMYCOMP_MAKE[] = { 0x02, 0xE0, 0x6B, };
__code unsigned char KEY_SET1_APPMYCOMP_BREAK[] = { 0x02, 0xE0, 0xEB, };
__code unsigned char KEY_SET2_APPMYCOMP_MAKE[] = { 0x02, 0xE0, 0x40, };
__code unsigned char KEY_SET2_APPMYCOMP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x40, };

__code unsigned char KEY_SET1_WWWSEARCH_MAKE[] = { 0x02, 0xE0, 0x65, };
__code unsigned char KEY_SET1_WWWSEARCH_BREAK[] = { 0x02, 0xE0, 0xE5, };
__code unsigned char KEY_SET2_WWWSEARCH_MAKE[] = { 0x02, 0xE0, 0x10, };
__code unsigned char KEY_SET2_WWWSEARCH_BREAK[] = { 0x03, 0xE0, 0xF0, 0x10, };

__code unsigned char KEY_SET1_WWWHOME_MAKE[] = { 0x02, 0xE0, 0x32, };
__code unsigned char KEY_SET1_WWWHOME_BREAK[] = { 0x02, 0xE0, 0xB2, };
__code unsigned char KEY_SET2_WWWHOME_MAKE[] = { 0x02, 0xE0, 0x3A, };
__code unsigned char KEY_SET2_WWWHOME_BREAK[] = { 0x03, 0xE0, 0xF0, 0x3A, };

__code unsigned char KEY_SET1_WWWBACK_MAKE[] = { 0x02, 0xE0, 0x6A, };
__code unsigned char KEY_SET1_WWWBACK_BREAK[] = { 0x02, 0xE0, 0xEA, };
__code unsigned char KEY_SET2_WWWBACK_MAKE[] = { 0x02, 0xE0, 0x38, };
__code unsigned char KEY_SET2_WWWBACK_BREAK[] = { 0x03, 0xE0, 0xF0, 0x38, };

__code unsigned char KEY_SET1_WWWFORWARD_MAKE[] = { 0x02, 0xE0, 0x69, };
__code unsigned char KEY_SET1_WWWFORWARD_BREAK[] = { 0x02, 0xE0, 0xE9, };
__code unsigned char KEY_SET2_WWWFORWARD_MAKE[] = { 0x02, 0xE0, 0x30, };
__code unsigned char KEY_SET2_WWWFORWARD_BREAK[] = { 0x03, 0xE0, 0xF0, 0x30, };

__code unsigned char KEY_SET1_WWWSTOP_MAKE[] = { 0x02, 0xE0, 0x68, };
__code unsigned char KEY_SET1_WWWSTOP_BREAK[] = { 0x02, 0xE0, 0xE8, };
__code unsigned char KEY_SET2_WWWSTOP_MAKE[] = { 0x02, 0xE0, 0x28, };
__code unsigned char KEY_SET2_WWWSTOP_BREAK[] = { 0x03, 0xE0, 0xF0, 0x28, };

__code unsigned char KEY_SET1_WWWREFRESH_MAKE[] = { 0x02, 0xE0, 0x67, };
__code unsigned char KEY_SET1_WWWREFRESH_BREAK[] = { 0x02, 0xE0, 0xE7, };
__code unsigned char KEY_SET2_WWWREFRESH_MAKE[] = { 0x02, 0xE0, 0x20, };
__code unsigned char KEY_SET2_WWWREFRESH_BREAK[] = { 0x03, 0xE0, 0xF0, 0x20, };

__code unsigned char KEY_SET1_WWWFAVE_MAKE[] = { 0x02, 0xE0, 0x66, };
__code unsigned char KEY_SET1_WWWFAVE_BREAK[] = { 0x02, 0xE0, 0xE6, };
__code unsigned char KEY_SET2_WWWFAVE_MAKE[] = { 0x02, 0xE0, 0x18, };
__code unsigned char KEY_SET2_WWWFAVE_BREAK[] = { 0x03, 0xE0, 0xF0, 0x18, };

__code unsigned char * __code HIDtoSET1_Make[] = {
	NULL, //0
	NULL, //1
	NULL, //2
	NULL, //3
	KEY_SET1_A_MAKE, //4
	KEY_SET1_B_MAKE, //5
	KEY_SET1_C_MAKE, //6
	KEY_SET1_D_MAKE, //7
	KEY_SET1_E_MAKE, //8
	KEY_SET1_F_MAKE, //9
	KEY_SET1_G_MAKE, //10
	KEY_SET1_H_MAKE, //11
	KEY_SET1_I_MAKE, //12
	KEY_SET1_J_MAKE, //13
	KEY_SET1_K_MAKE, //14
	KEY_SET1_L_MAKE, //15
	KEY_SET1_M_MAKE, //16
	KEY_SET1_N_MAKE, //17
	KEY_SET1_O_MAKE, //18
	KEY_SET1_P_MAKE, //19
	KEY_SET1_Q_MAKE, //20
	KEY_SET1_R_MAKE, //21
	KEY_SET1_S_MAKE, //22
	KEY_SET1_T_MAKE, //23
	KEY_SET1_U_MAKE, //24
	KEY_SET1_V_MAKE, //25
	KEY_SET1_W_MAKE, //26
	KEY_SET1_X_MAKE, //27
	KEY_SET1_Y_MAKE, //28
	KEY_SET1_Z_MAKE, //29
	KEY_SET1_1_MAKE, //30
	KEY_SET1_2_MAKE, //31
	KEY_SET1_3_MAKE, //32
	KEY_SET1_4_MAKE, //33
	KEY_SET1_5_MAKE, //34
	KEY_SET1_6_MAKE, //35
	KEY_SET1_7_MAKE, //36
	KEY_SET1_8_MAKE, //37
	KEY_SET1_9_MAKE, //38
	KEY_SET1_0_MAKE, //39
	KEY_SET1_ENTER_MAKE, //40
	KEY_SET1_ESCAPE_MAKE, //41
	KEY_SET1_BKSPACE_MAKE, //42
	KEY_SET1_TAB_MAKE, //43
	KEY_SET1_SPACE_MAKE, //44
	KEY_SET1_DASH_MAKE, //45
	KEY_SET1_EQUALS_MAKE, //46
	KEY_SET1_LEFTSQB_MAKE, //47
	KEY_SET1_RIGHTSQB_MAKE, //48
	KEY_SET1_BKSLASH_MAKE, //49
	KEY_SET1_EURO1_MAKE, //50
	KEY_SET1_SEMICOLON_MAKE, //51
	KEY_SET1_APOSTROPHE_MAKE, //52
	KEY_SET1_TILDE_MAKE, //53
	KEY_SET1_COMMA_MAKE, //54
	KEY_SET1_PERIOD_MAKE, //55
	KEY_SET1_FWSLASH_MAKE, //56
	KEY_SET1_CAPS_MAKE, //57
	KEY_SET1_F1_MAKE, //58
	KEY_SET1_F2_MAKE, //59
	KEY_SET1_F3_MAKE, //60
	KEY_SET1_F4_MAKE, //61
	KEY_SET1_F5_MAKE, //62
	KEY_SET1_F6_MAKE, //63
	KEY_SET1_F7_MAKE, //64
	KEY_SET1_F8_MAKE, //65
	KEY_SET1_F9_MAKE, //66
	KEY_SET1_F10_MAKE, //67
	KEY_SET1_F11_MAKE, //68
	KEY_SET1_F12_MAKE, //69
	KEY_SET1_PRTSCR_MAKE, //70
	KEY_SET1_SCRLOCK_MAKE, //71
	KEY_SET1_PAUSE_MAKE, //72
	KEY_SET1_INSERT_MAKE, //73
	KEY_SET1_HOME_MAKE, //74
	KEY_SET1_PGUP_MAKE, //75
	KEY_SET1_DELETE_MAKE, //76
	KEY_SET1_END_MAKE, //77
	KEY_SET1_PGDN_MAKE, //78
	KEY_SET1_RIGHT_MAKE, //79
	KEY_SET1_LEFT_MAKE, //80
	KEY_SET1_DOWN_MAKE, //81
	KEY_SET1_UP_MAKE, //82
	KEY_SET1_NUMLCK_MAKE, //83
	KEY_SET1_PADFWSLASH_MAKE, //84
	KEY_SET1_PADASTERISK_MAKE, //85
	KEY_SET1_PADMINUS_MAKE, //86
	KEY_SET1_PADPLUS_MAKE, //87
	KEY_SET1_PADENTER_MAKE, //88
	KEY_SET1_PAD1_MAKE, //89
	KEY_SET1_PAD2_MAKE, //90
	KEY_SET1_PAD3_MAKE, //91
	KEY_SET1_PAD4_MAKE, //92
	KEY_SET1_PAD5_MAKE, //93
	KEY_SET1_PAD6_MAKE, //94
	KEY_SET1_PAD7_MAKE, //95
	KEY_SET1_PAD8_MAKE, //96
	KEY_SET1_PAD9_MAKE, //97
	KEY_SET1_PAD0_MAKE, //98
	KEY_SET1_PADPERIOD_MAKE, //99
	KEY_SET1_EURO2_MAKE, //100
	KEY_SET1_APP_MAKE, //101
	KEY_SET1_KEYPOWER_MAKE, //102
	KEY_SET1_PADEQUALS_MAKE, //103
	KEY_SET1_F13_MAKE, //104
	KEY_SET1_F14_MAKE, //105
	KEY_SET1_F15_MAKE, //106
	KEY_SET1_F16_MAKE, //107
	KEY_SET1_F17_MAKE, //108
	KEY_SET1_F18_MAKE, //109
	KEY_SET1_F19_MAKE, //110
	KEY_SET1_F20_MAKE, //111
	KEY_SET1_F21_MAKE, //112
	KEY_SET1_F22_MAKE, //113
	KEY_SET1_F23_MAKE, //114
	KEY_SET1_F24_MAKE, //115
	NULL, //116
	NULL, //117
	NULL, //118
	NULL, //119
	NULL, //120
	NULL, //121
	NULL, //122
	NULL, //123
	NULL, //124
	NULL, //125
	NULL, //126
	NULL, //127
	NULL, //128
	NULL, //129
	NULL, //130
	NULL, //131
	NULL, //132
	KEY_SET1_PADCOMMA_MAKE, //133
	NULL, //134
	KEY_SET1_INTL1_MAKE, //135
	KEY_SET1_INTL2_MAKE, //136
	KEY_SET1_INTL3_MAKE, //137
	KEY_SET1_INTL4_MAKE, //138
	KEY_SET1_INTL5_MAKE, //139
	KEY_SET1_INTL6_MAKE, //140
	NULL, //141
	NULL, //142
	NULL, //143
	KEY_SET1_LANG1_MAKE, //144
	KEY_SET1_LANG2_MAKE, //145
	KEY_SET1_LANG3_MAKE, //146
	KEY_SET1_LANG4_MAKE, //147
	KEY_SET1_LANG5_MAKE, //148
	NULL, //149
	NULL, //150
	NULL, //151
	NULL, //152
	NULL, //153
	NULL, //154
	NULL, //155
	NULL, //156
	NULL, //157
	NULL, //158
	NULL, //159
	NULL, //160
	NULL, //161
	NULL, //162
	NULL, //163
	NULL, //164
	NULL, //165
	NULL, //166
	NULL, //167
	NULL, //168
	NULL, //169
	NULL, //170
	NULL, //171
	NULL, //172
	NULL, //173
	NULL, //174
	NULL, //175
	NULL, //176
	NULL, //177
	NULL, //178
	NULL, //179
	NULL, //180
	NULL, //181
	NULL, //182
	NULL, //183
	NULL, //184
	NULL, //185
	NULL, //186
	NULL, //187
	NULL, //188
	NULL, //189
	NULL, //190
	NULL, //191
	NULL, //192
	NULL, //193
	NULL, //194
	NULL, //195
	NULL, //196
	NULL, //197
	NULL, //198
	NULL, //199
	NULL, //200
	NULL, //201
	NULL, //202
	NULL, //203
	NULL, //204
	NULL, //205
	NULL, //206
	NULL, //207
	NULL, //208
	NULL, //209
	NULL, //210
	NULL, //211
	NULL, //212
	NULL, //213
	NULL, //214
	NULL, //215
	NULL, //216
	NULL, //217
	NULL, //218
	NULL, //219
	NULL, //220
	NULL, //221
	NULL, //222
	NULL, //223
	KEY_SET1_LCTRL_MAKE, //224
	KEY_SET1_LSHIFT_MAKE, //225
	KEY_SET1_LALT_MAKE, //226
	KEY_SET1_LGUI_MAKE, //227
	KEY_SET1_RCTRL_MAKE, //228
	KEY_SET1_RSHIFT_MAKE, //229
	KEY_SET1_RALT_MAKE, //230
	KEY_SET1_RGUI_MAKE, //231
	NULL, //232
	NULL, //233
	NULL, //234
	NULL, //235
	NULL, //236
	NULL, //237
	NULL, //238
	NULL, //239
	NULL, //240
	NULL, //241
	NULL, //242
	NULL, //243
	NULL, //244
	NULL, //245
	NULL, //246
	NULL, //247
	NULL, //248
	NULL, //249
	NULL, //250
	NULL, //251
	NULL, //252
	NULL, //253
	NULL, //254
	NULL, //255
};

__code unsigned char * __code HIDtoSET1_Break[] = {
	NULL, //0
	NULL, //1
	NULL, //2
	NULL, //3
	KEY_SET1_A_BREAK, //4
	KEY_SET1_B_BREAK, //5
	KEY_SET1_C_BREAK, //6
	KEY_SET1_D_BREAK, //7
	KEY_SET1_E_BREAK, //8
	KEY_SET1_F_BREAK, //9
	KEY_SET1_G_BREAK, //10
	KEY_SET1_H_BREAK, //11
	KEY_SET1_I_BREAK, //12
	KEY_SET1_J_BREAK, //13
	KEY_SET1_K_BREAK, //14
	KEY_SET1_L_BREAK, //15
	KEY_SET1_M_BREAK, //16
	KEY_SET1_N_BREAK, //17
	KEY_SET1_O_BREAK, //18
	KEY_SET1_P_BREAK, //19
	KEY_SET1_Q_BREAK, //20
	KEY_SET1_R_BREAK, //21
	KEY_SET1_S_BREAK, //22
	KEY_SET1_T_BREAK, //23
	KEY_SET1_U_BREAK, //24
	KEY_SET1_V_BREAK, //25
	KEY_SET1_W_BREAK, //26
	KEY_SET1_X_BREAK, //27
	KEY_SET1_Y_BREAK, //28
	KEY_SET1_Z_BREAK, //29
	KEY_SET1_1_BREAK, //30
	KEY_SET1_2_BREAK, //31
	KEY_SET1_3_BREAK, //32
	KEY_SET1_4_BREAK, //33
	KEY_SET1_5_BREAK, //34
	KEY_SET1_6_BREAK, //35
	KEY_SET1_7_BREAK, //36
	KEY_SET1_8_BREAK, //37
	KEY_SET1_9_BREAK, //38
	KEY_SET1_0_BREAK, //39
	KEY_SET1_ENTER_BREAK, //40
	KEY_SET1_ESCAPE_BREAK, //41
	KEY_SET1_BKSPACE_BREAK, //42
	KEY_SET1_TAB_BREAK, //43
	KEY_SET1_SPACE_BREAK, //44
	KEY_SET1_DASH_BREAK, //45
	KEY_SET1_EQUALS_BREAK, //46
	KEY_SET1_LEFTSQB_BREAK, //47
	KEY_SET1_RIGHTSQB_BREAK, //48
	KEY_SET1_BKSLASH_BREAK, //49
	KEY_SET1_EURO1_BREAK, //50
	KEY_SET1_SEMICOLON_BREAK, //51
	KEY_SET1_APOSTROPHE_BREAK, //52
	KEY_SET1_TILDE_BREAK, //53
	KEY_SET1_COMMA_BREAK, //54
	KEY_SET1_PERIOD_BREAK, //55
	KEY_SET1_FWSLASH_BREAK, //56
	KEY_SET1_CAPS_BREAK, //57
	KEY_SET1_F1_BREAK, //58
	KEY_SET1_F2_BREAK, //59
	KEY_SET1_F3_BREAK, //60
	KEY_SET1_F4_BREAK, //61
	KEY_SET1_F5_BREAK, //62
	KEY_SET1_F6_BREAK, //63
	KEY_SET1_F7_BREAK, //64
	KEY_SET1_F8_BREAK, //65
	KEY_SET1_F9_BREAK, //66
	KEY_SET1_F10_BREAK, //67
	KEY_SET1_F11_BREAK, //68
	KEY_SET1_F12_BREAK, //69
	KEY_SET1_PRTSCR_BREAK, //70
	KEY_SET1_SCRLOCK_BREAK, //71
	NULL, //72
	KEY_SET1_INSERT_BREAK, //73
	KEY_SET1_HOME_BREAK, //74
	KEY_SET1_PGUP_BREAK, //75
	KEY_SET1_DELETE_BREAK, //76
	KEY_SET1_END_BREAK, //77
	KEY_SET1_PGDN_BREAK, //78
	KEY_SET1_RIGHT_BREAK, //79
	KEY_SET1_LEFT_BREAK, //80
	KEY_SET1_DOWN_BREAK, //81
	KEY_SET1_UP_BREAK, //82
	KEY_SET1_NUMLCK_BREAK, //83
	KEY_SET1_PADFWSLASH_BREAK, //84
	KEY_SET1_PADASTERISK_BREAK, //85
	KEY_SET1_PADMINUS_BREAK, //86
	KEY_SET1_PADPLUS_BREAK, //87
	KEY_SET1_PADENTER_BREAK, //88
	KEY_SET1_PAD1_BREAK, //89
	KEY_SET1_PAD2_BREAK, //90
	KEY_SET1_PAD3_BREAK, //91
	KEY_SET1_PAD4_BREAK, //92
	KEY_SET1_PAD5_BREAK, //93
	KEY_SET1_PAD6_BREAK, //94
	KEY_SET1_PAD7_BREAK, //95
	KEY_SET1_PAD8_BREAK, //96
	KEY_SET1_PAD9_BREAK, //97
	KEY_SET1_PAD0_BREAK, //98
	KEY_SET1_PADPERIOD_BREAK, //99
	KEY_SET1_EURO2_BREAK, //100
	KEY_SET1_APP_BREAK, //101
	KEY_SET1_KEYPOWER_BREAK, //102
	KEY_SET1_PADEQUALS_BREAK, //103
	KEY_SET1_F13_BREAK, //104
	KEY_SET1_F14_BREAK, //105
	KEY_SET1_F15_BREAK, //106
	KEY_SET1_F16_BREAK, //107
	KEY_SET1_F17_BREAK, //108
	KEY_SET1_F18_BREAK, //109
	KEY_SET1_F19_BREAK, //110
	KEY_SET1_F20_BREAK, //111
	KEY_SET1_F21_BREAK, //112
	KEY_SET1_F22_BREAK, //113
	KEY_SET1_F23_BREAK, //114
	KEY_SET1_F24_BREAK, //115
	NULL, //116
	NULL, //117
	NULL, //118
	NULL, //119
	NULL, //120
	NULL, //121
	NULL, //122
	NULL, //123
	NULL, //124
	NULL, //125
	NULL, //126
	NULL, //127
	NULL, //128
	NULL, //129
	NULL, //130
	NULL, //131
	NULL, //132
	KEY_SET1_PADCOMMA_BREAK, //133
	NULL, //134
	KEY_SET1_INTL1_BREAK, //135
	KEY_SET1_INTL2_BREAK, //136
	KEY_SET1_INTL3_BREAK, //137
	KEY_SET1_INTL4_BREAK, //138
	KEY_SET1_INTL5_BREAK, //139
	KEY_SET1_INTL6_BREAK, //140
	NULL, //141
	NULL, //142
	NULL, //143
	NULL, //144
	NULL, //145
	KEY_SET1_LANG3_BREAK, //146
	KEY_SET1_LANG4_BREAK, //147
	KEY_SET1_LANG5_BREAK, //148
	NULL, //149
	NULL, //150
	NULL, //151
	NULL, //152
	NULL, //153
	NULL, //154
	NULL, //155
	NULL, //156
	NULL, //157
	NULL, //158
	NULL, //159
	NULL, //160
	NULL, //161
	NULL, //162
	NULL, //163
	NULL, //164
	NULL, //165
	NULL, //166
	NULL, //167
	NULL, //168
	NULL, //169
	NULL, //170
	NULL, //171
	NULL, //172
	NULL, //173
	NULL, //174
	NULL, //175
	NULL, //176
	NULL, //177
	NULL, //178
	NULL, //179
	NULL, //180
	NULL, //181
	NULL, //182
	NULL, //183
	NULL, //184
	NULL, //185
	NULL, //186
	NULL, //187
	NULL, //188
	NULL, //189
	NULL, //190
	NULL, //191
	NULL, //192
	NULL, //193
	NULL, //194
	NULL, //195
	NULL, //196
	NULL, //197
	NULL, //198
	NULL, //199
	NULL, //200
	NULL, //201
	NULL, //202
	NULL, //203
	NULL, //204
	NULL, //205
	NULL, //206
	NULL, //207
	NULL, //208
	NULL, //209
	NULL, //210
	NULL, //211
	NULL, //212
	NULL, //213
	NULL, //214
	NULL, //215
	NULL, //216
	NULL, //217
	NULL, //218
	NULL, //219
	NULL, //220
	NULL, //221
	NULL, //222
	NULL, //223
	KEY_SET1_LCTRL_BREAK, //224
	KEY_SET1_LSHIFT_BREAK, //225
	KEY_SET1_LALT_BREAK, //226
	KEY_SET1_LGUI_BREAK, //227
	KEY_SET1_RCTRL_BREAK, //228
	KEY_SET1_RSHIFT_BREAK, //229
	KEY_SET1_RALT_BREAK, //230
	KEY_SET1_RGUI_BREAK, //231
	NULL, //232
	NULL, //233
	NULL, //234
	NULL, //235
	NULL, //236
	NULL, //237
	NULL, //238
	NULL, //239
	NULL, //240
	NULL, //241
	NULL, //242
	NULL, //243
	NULL, //244
	NULL, //245
	NULL, //246
	NULL, //247
	NULL, //248
	NULL, //249
	NULL, //250
	NULL, //251
	NULL, //252
	NULL, //253
	NULL, //254
	NULL, //255
};

__code unsigned char * __code HIDtoSET2_Make[] = {
	NULL, //0
	NULL, //1
	NULL, //2
	NULL, //3
	KEY_SET2_A_MAKE, //4
	KEY_SET2_B_MAKE, //5
	KEY_SET2_C_MAKE, //6
	KEY_SET2_D_MAKE, //7
	KEY_SET2_E_MAKE, //8
	KEY_SET2_F_MAKE, //9
	KEY_SET2_G_MAKE, //10
	KEY_SET2_H_MAKE, //11
	KEY_SET2_I_MAKE, //12
	KEY_SET2_J_MAKE, //13
	KEY_SET2_K_MAKE, //14
	KEY_SET2_L_MAKE, //15
	KEY_SET2_M_MAKE, //16
	KEY_SET2_N_MAKE, //17
	KEY_SET2_O_MAKE, //18
	KEY_SET2_P_MAKE, //19
	KEY_SET2_Q_MAKE, //20
	KEY_SET2_R_MAKE, //21
	KEY_SET2_S_MAKE, //22
	KEY_SET2_T_MAKE, //23
	KEY_SET2_U_MAKE, //24
	KEY_SET2_V_MAKE, //25
	KEY_SET2_W_MAKE, //26
	KEY_SET2_X_MAKE, //27
	KEY_SET2_Y_MAKE, //28
	KEY_SET2_Z_MAKE, //29
	KEY_SET2_1_MAKE, //30
	KEY_SET2_2_MAKE, //31
	KEY_SET2_3_MAKE, //32
	KEY_SET2_4_MAKE, //33
	KEY_SET2_5_MAKE, //34
	KEY_SET2_6_MAKE, //35
	KEY_SET2_7_MAKE, //36
	KEY_SET2_8_MAKE, //37
	KEY_SET2_9_MAKE, //38
	KEY_SET2_0_MAKE, //39
	KEY_SET2_ENTER_MAKE, //40
	KEY_SET2_ESCAPE_MAKE, //41
	KEY_SET2_BKSPACE_MAKE, //42
	KEY_SET2_TAB_MAKE, //43
	KEY_SET2_SPACE_MAKE, //44
	KEY_SET2_DASH_MAKE, //45
	KEY_SET2_EQUALS_MAKE, //46
	KEY_SET2_LEFTSQB_MAKE, //47
	KEY_SET2_RIGHTSQB_MAKE, //48
	KEY_SET2_BKSLASH_MAKE, //49
	KEY_SET2_EURO1_MAKE, //50
	KEY_SET2_SEMICOLON_MAKE, //51
	KEY_SET2_APOSTROPHE_MAKE, //52
	KEY_SET2_TILDE_MAKE, //53
	KEY_SET2_COMMA_MAKE, //54
	KEY_SET2_PERIOD_MAKE, //55
	KEY_SET2_FWSLASH_MAKE, //56
	KEY_SET2_CAPS_MAKE, //57
	KEY_SET2_F1_MAKE, //58
	KEY_SET2_F2_MAKE, //59
	KEY_SET2_F3_MAKE, //60
	KEY_SET2_F4_MAKE, //61
	KEY_SET2_F5_MAKE, //62
	KEY_SET2_F6_MAKE, //63
	KEY_SET2_F7_MAKE, //64
	KEY_SET2_F8_MAKE, //65
	KEY_SET2_F9_MAKE, //66
	KEY_SET2_F10_MAKE, //67
	KEY_SET2_F11_MAKE, //68
	KEY_SET2_F12_MAKE, //69
	KEY_SET2_PRTSCR_MAKE, //70
	KEY_SET2_SCRLOCK_MAKE, //71
	KEY_SET2_PAUSE_MAKE, //72
	KEY_SET2_INSERT_MAKE, //73
	KEY_SET2_HOME_MAKE, //74
	KEY_SET2_PGUP_MAKE, //75
	KEY_SET2_DELETE_MAKE, //76
	KEY_SET2_END_MAKE, //77
	KEY_SET2_PGDN_MAKE, //78
	KEY_SET2_RIGHT_MAKE, //79
	KEY_SET2_LEFT_MAKE, //80
	KEY_SET2_DOWN_MAKE, //81
	KEY_SET2_UP_MAKE, //82
	KEY_SET2_NUMLCK_MAKE, //83
	KEY_SET2_PADFWSLASH_MAKE, //84
	KEY_SET2_PADASTERISK_MAKE, //85
	KEY_SET2_PADMINUS_MAKE, //86
	KEY_SET2_PADPLUS_MAKE, //87
	KEY_SET2_PADENTER_MAKE, //88
	KEY_SET2_PAD1_MAKE, //89
	KEY_SET2_PAD2_MAKE, //90
	KEY_SET2_PAD3_MAKE, //91
	KEY_SET2_PAD4_MAKE, //92
	KEY_SET2_PAD5_MAKE, //93
	KEY_SET2_PAD6_MAKE, //94
	KEY_SET2_PAD7_MAKE, //95
	KEY_SET2_PAD8_MAKE, //96
	KEY_SET2_PAD9_MAKE, //97
	KEY_SET2_PAD0_MAKE, //98
	KEY_SET2_PADPERIOD_MAKE, //99
	KEY_SET2_EURO2_MAKE, //100
	KEY_SET2_APP_MAKE, //101
	KEY_SET2_KEYPOWER_MAKE, //102
	KEY_SET2_PADEQUALS_MAKE, //103
	KEY_SET2_F13_MAKE, //104
	KEY_SET2_F14_MAKE, //105
	KEY_SET2_F15_MAKE, //106
	KEY_SET2_F16_MAKE, //107
	KEY_SET2_F17_MAKE, //108
	KEY_SET2_F18_MAKE, //109
	KEY_SET2_F19_MAKE, //110
	KEY_SET2_F20_MAKE, //111
	KEY_SET2_F21_MAKE, //112
	KEY_SET2_F22_MAKE, //113
	KEY_SET2_F23_MAKE, //114
	KEY_SET2_F24_MAKE, //115
	NULL, //116
	NULL, //117
	NULL, //118
	NULL, //119
	NULL, //120
	NULL, //121
	NULL, //122
	NULL, //123
	NULL, //124
	NULL, //125
	NULL, //126
	NULL, //127
	NULL, //128
	NULL, //129
	NULL, //130
	NULL, //131
	NULL, //132
	KEY_SET2_PADCOMMA_MAKE, //133
	NULL, //134
	KEY_SET2_INTL1_MAKE, //135
	KEY_SET2_INTL2_MAKE, //136
	KEY_SET2_INTL3_MAKE, //137
	KEY_SET2_INTL4_MAKE, //138
	KEY_SET2_INTL5_MAKE, //139
	KEY_SET2_INTL6_MAKE, //140
	NULL, //141
	NULL, //142
	NULL, //143
	KEY_SET2_LANG1_MAKE, //144
	KEY_SET2_LANG2_MAKE, //145
	KEY_SET2_LANG3_MAKE, //146
	KEY_SET2_LANG4_MAKE, //147
	KEY_SET2_LANG5_MAKE, //148
	NULL, //149
	NULL, //150
	NULL, //151
	NULL, //152
	NULL, //153
	NULL, //154
	NULL, //155
	NULL, //156
	NULL, //157
	NULL, //158
	NULL, //159
	NULL, //160
	NULL, //161
	NULL, //162
	NULL, //163
	NULL, //164
	NULL, //165
	NULL, //166
	NULL, //167
	NULL, //168
	NULL, //169
	NULL, //170
	NULL, //171
	NULL, //172
	NULL, //173
	NULL, //174
	NULL, //175
	NULL, //176
	NULL, //177
	NULL, //178
	NULL, //179
	NULL, //180
	NULL, //181
	NULL, //182
	NULL, //183
	NULL, //184
	NULL, //185
	NULL, //186
	NULL, //187
	NULL, //188
	NULL, //189
	NULL, //190
	NULL, //191
	NULL, //192
	NULL, //193
	NULL, //194
	NULL, //195
	NULL, //196
	NULL, //197
	NULL, //198
	NULL, //199
	NULL, //200
	NULL, //201
	NULL, //202
	NULL, //203
	NULL, //204
	NULL, //205
	NULL, //206
	NULL, //207
	NULL, //208
	NULL, //209
	NULL, //210
	NULL, //211
	NULL, //212
	NULL, //213
	NULL, //214
	NULL, //215
	NULL, //216
	NULL, //217
	NULL, //218
	NULL, //219
	NULL, //220
	NULL, //221
	NULL, //222
	NULL, //223
	KEY_SET2_LCTRL_MAKE, //224
	KEY_SET2_LSHIFT_MAKE, //225
	KEY_SET2_LALT_MAKE, //226
	KEY_SET2_LGUI_MAKE, //227
	KEY_SET2_RCTRL_MAKE, //228
	KEY_SET2_RSHIFT_MAKE, //229
	KEY_SET2_RALT_MAKE, //230
	KEY_SET2_RGUI_MAKE, //231
	NULL, //232
	NULL, //233
	NULL, //234
	NULL, //235
	NULL, //236
	NULL, //237
	NULL, //238
	NULL, //239
	NULL, //240
	NULL, //241
	NULL, //242
	NULL, //243
	NULL, //244
	NULL, //245
	NULL, //246
	NULL, //247
	NULL, //248
	NULL, //249
	NULL, //250
	NULL, //251
	NULL, //252
	NULL, //253
	NULL, //254
	NULL, //255
};

__code unsigned char * __code HIDtoSET2_Break[] = {
	NULL, //0
	NULL, //1
	NULL, //2
	NULL, //3
	KEY_SET2_A_BREAK, //4
	KEY_SET2_B_BREAK, //5
	KEY_SET2_C_BREAK, //6
	KEY_SET2_D_BREAK, //7
	KEY_SET2_E_BREAK, //8
	KEY_SET2_F_BREAK, //9
	KEY_SET2_G_BREAK, //10
	KEY_SET2_H_BREAK, //11
	KEY_SET2_I_BREAK, //12
	KEY_SET2_J_BREAK, //13
	KEY_SET2_K_BREAK, //14
	KEY_SET2_L_BREAK, //15
	KEY_SET2_M_BREAK, //16
	KEY_SET2_N_BREAK, //17
	KEY_SET2_O_BREAK, //18
	KEY_SET2_P_BREAK, //19
	KEY_SET2_Q_BREAK, //20
	KEY_SET2_R_BREAK, //21
	KEY_SET2_S_BREAK, //22
	KEY_SET2_T_BREAK, //23
	KEY_SET2_U_BREAK, //24
	KEY_SET2_V_BREAK, //25
	KEY_SET2_W_BREAK, //26
	KEY_SET2_X_BREAK, //27
	KEY_SET2_Y_BREAK, //28
	KEY_SET2_Z_BREAK, //29
	KEY_SET2_1_BREAK, //30
	KEY_SET2_2_BREAK, //31
	KEY_SET2_3_BREAK, //32
	KEY_SET2_4_BREAK, //33
	KEY_SET2_5_BREAK, //34
	KEY_SET2_6_BREAK, //35
	KEY_SET2_7_BREAK, //36
	KEY_SET2_8_BREAK, //37
	KEY_SET2_9_BREAK, //38
	KEY_SET2_0_BREAK, //39
	KEY_SET2_ENTER_BREAK, //40
	KEY_SET2_ESCAPE_BREAK, //41
	KEY_SET2_BKSPACE_BREAK, //42
	KEY_SET2_TAB_BREAK, //43
	KEY_SET2_SPACE_BREAK, //44
	KEY_SET2_DASH_BREAK, //45
	KEY_SET2_EQUALS_BREAK, //46
	KEY_SET2_LEFTSQB_BREAK, //47
	KEY_SET2_RIGHTSQB_BREAK, //48
	KEY_SET2_BKSLASH_BREAK, //49
	KEY_SET2_EURO1_BREAK, //50
	KEY_SET2_SEMICOLON_BREAK, //51
	KEY_SET2_APOSTROPHE_BREAK, //52
	KEY_SET2_TILDE_BREAK, //53
	KEY_SET2_COMMA_BREAK, //54
	KEY_SET2_PERIOD_BREAK, //55
	KEY_SET2_FWSLASH_BREAK, //56
	KEY_SET2_CAPS_BREAK, //57
	KEY_SET2_F1_BREAK, //58
	KEY_SET2_F2_BREAK, //59
	KEY_SET2_F3_BREAK, //60
	KEY_SET2_F4_BREAK, //61
	KEY_SET2_F5_BREAK, //62
	KEY_SET2_F6_BREAK, //63
	KEY_SET2_F7_BREAK, //64
	KEY_SET2_F8_BREAK, //65
	KEY_SET2_F9_BREAK, //66
	KEY_SET2_F10_BREAK, //67
	KEY_SET2_F11_BREAK, //68
	KEY_SET2_F12_BREAK, //69
	KEY_SET2_PRTSCR_BREAK, //70
	KEY_SET2_SCRLOCK_BREAK, //71
	NULL, //72
	KEY_SET2_INSERT_BREAK, //73
	KEY_SET2_HOME_BREAK, //74
	KEY_SET2_PGUP_BREAK, //75
	KEY_SET2_DELETE_BREAK, //76
	KEY_SET2_END_BREAK, //77
	KEY_SET2_PGDN_BREAK, //78
	KEY_SET2_RIGHT_BREAK, //79
	KEY_SET2_LEFT_BREAK, //80
	KEY_SET2_DOWN_BREAK, //81
	KEY_SET2_UP_BREAK, //82
	KEY_SET2_NUMLCK_BREAK, //83
	KEY_SET2_PADFWSLASH_BREAK, //84
	KEY_SET2_PADASTERISK_BREAK, //85
	KEY_SET2_PADMINUS_BREAK, //86
	KEY_SET2_PADPLUS_BREAK, //87
	KEY_SET2_PADENTER_BREAK, //88
	KEY_SET2_PAD1_BREAK, //89
	KEY_SET2_PAD2_BREAK, //90
	KEY_SET2_PAD3_BREAK, //91
	KEY_SET2_PAD4_BREAK, //92
	KEY_SET2_PAD5_BREAK, //93
	KEY_SET2_PAD6_BREAK, //94
	KEY_SET2_PAD7_BREAK, //95
	KEY_SET2_PAD8_BREAK, //96
	KEY_SET2_PAD9_BREAK, //97
	KEY_SET2_PAD0_BREAK, //98
	KEY_SET2_PADPERIOD_BREAK, //99
	KEY_SET2_EURO2_BREAK, //100
	KEY_SET2_APP_BREAK, //101
	KEY_SET2_KEYPOWER_BREAK, //102
	KEY_SET2_PADEQUALS_BREAK, //103
	KEY_SET2_F13_BREAK, //104
	KEY_SET2_F14_BREAK, //105
	KEY_SET2_F15_BREAK, //106
	KEY_SET2_F16_BREAK, //107
	KEY_SET2_F17_BREAK, //108
	KEY_SET2_F18_BREAK, //109
	KEY_SET2_F19_BREAK, //110
	KEY_SET2_F20_BREAK, //111
	KEY_SET2_F21_BREAK, //112
	KEY_SET2_F22_BREAK, //113
	KEY_SET2_F23_BREAK, //114
	KEY_SET2_F24_BREAK, //115
	NULL, //116
	NULL, //117
	NULL, //118
	NULL, //119
	NULL, //120
	NULL, //121
	NULL, //122
	NULL, //123
	NULL, //124
	NULL, //125
	NULL, //126
	NULL, //127
	NULL, //128
	NULL, //129
	NULL, //130
	NULL, //131
	NULL, //132
	KEY_SET2_PADCOMMA_BREAK, //133
	NULL, //134
	KEY_SET2_INTL1_BREAK, //135
	KEY_SET2_INTL2_BREAK, //136
	KEY_SET2_INTL3_BREAK, //137
	KEY_SET2_INTL4_BREAK, //138
	KEY_SET2_INTL5_BREAK, //139
	KEY_SET2_INTL6_BREAK, //140
	NULL, //141
	NULL, //142
	NULL, //143
	NULL, //144
	NULL, //145
	KEY_SET2_LANG3_BREAK, //146
	KEY_SET2_LANG4_BREAK, //147
	KEY_SET2_LANG5_BREAK, //148
	NULL, //149
	NULL, //150
	NULL, //151
	NULL, //152
	NULL, //153
	NULL, //154
	NULL, //155
	NULL, //156
	NULL, //157
	NULL, //158
	NULL, //159
	NULL, //160
	NULL, //161
	NULL, //162
	NULL, //163
	NULL, //164
	NULL, //165
	NULL, //166
	NULL, //167
	NULL, //168
	NULL, //169
	NULL, //170
	NULL, //171
	NULL, //172
	NULL, //173
	NULL, //174
	NULL, //175
	NULL, //176
	NULL, //177
	NULL, //178
	NULL, //179
	NULL, //180
	NULL, //181
	NULL, //182
	NULL, //183
	NULL, //184
	NULL, //185
	NULL, //186
	NULL, //187
	NULL, //188
	NULL, //189
	NULL, //190
	NULL, //191
	NULL, //192
	NULL, //193
	NULL, //194
	NULL, //195
	NULL, //196
	NULL, //197
	NULL, //198
	NULL, //199
	NULL, //200
	NULL, //201
	NULL, //202
	NULL, //203
	NULL, //204
	NULL, //205
	NULL, //206
	NULL, //207
	NULL, //208
	NULL, //209
	NULL, //210
	NULL, //211
	NULL, //212
	NULL, //213
	NULL, //214
	NULL, //215
	NULL, //216
	NULL, //217
	NULL, //218
	NULL, //219
	NULL, //220
	NULL, //221
	NULL, //222
	NULL, //223
	KEY_SET2_LCTRL_BREAK, //224
	KEY_SET2_LSHIFT_BREAK, //225
	KEY_SET2_LALT_BREAK, //226
	KEY_SET2_LGUI_BREAK, //227
	KEY_SET2_RCTRL_BREAK, //228
	KEY_SET2_RSHIFT_BREAK, //229
	KEY_SET2_RALT_BREAK, //230
	KEY_SET2_RGUI_BREAK, //231
	NULL, //232
	NULL, //233
	NULL, //234
	NULL, //235
	NULL, //236
	NULL, //237
	NULL, //238
	NULL, //239
	NULL, //240
	NULL, //241
	NULL, //242
	NULL, //243
	NULL, //244
	NULL, //245
	NULL, //246
	NULL, //247
	NULL, //248
	NULL, //249
	NULL, //250
	NULL, //251
	NULL, //252
	NULL, //253
	NULL, //254
	NULL, //255
};

const EXTCHARLOOKUP HID0CtoSET1_Make[] = {
	{181, KEY_SET1_MEDIANEXT_MAKE}, //181
	{182, KEY_SET1_MEDIAPREV_MAKE}, //182
	{183, KEY_SET1_MEDIASTOP_MAKE}, //183
	{205, KEY_SET1_MEDIANPLAY_MAKE}, //205
	{226, KEY_SET1_MEDIAMUTE_MAKE}, //226
	{233, KEY_SET1_VOLUP_MAKE}, //233
	{234, KEY_SET1_VOLDN_MAKE}, //234
	{387, KEY_SET1_MEDSELECT_MAKE}, //387
	{394, KEY_SET1_APPMAIL_MAKE}, //394
	{402, KEY_SET1_APPCALC_MAKE}, //402
	{404, KEY_SET1_APPMYCOMP_MAKE}, //404
	{545, KEY_SET1_WWWSEARCH_MAKE}, //545
	{547, KEY_SET1_WWWHOME_MAKE}, //547
	{548, KEY_SET1_WWWBACK_MAKE}, //548
	{549, KEY_SET1_WWWFORWARD_MAKE}, //549
	{550, KEY_SET1_WWWSTOP_MAKE}, //550
	{551, KEY_SET1_WWWREFRESH_MAKE}, //551
	{554, KEY_SET1_WWWFAVE_MAKE}, //554
	{NULL, NULL}
};

const EXTCHARLOOKUP HID0CtoSET1_Break[] = {
	{181, KEY_SET1_MEDIANEXT_BREAK}, //181
	{182, KEY_SET1_MEDIAPREV_BREAK}, //182
	{183, KEY_SET1_MEDIASTOP_BREAK}, //183
	{205, KEY_SET1_MEDIANPLAY_BREAK}, //205
	{226, KEY_SET1_MEDIAMUTE_BREAK}, //226
	{233, KEY_SET1_VOLUP_BREAK}, //233
	{234, KEY_SET1_VOLDN_BREAK}, //234
	{387, KEY_SET1_MEDSELECT_BREAK}, //387
	{394, KEY_SET1_APPMAIL_BREAK}, //394
	{402, KEY_SET1_APPCALC_BREAK}, //402
	{404, KEY_SET1_APPMYCOMP_BREAK}, //404
	{545, KEY_SET1_WWWSEARCH_BREAK}, //545
	{547, KEY_SET1_WWWHOME_BREAK}, //547
	{548, KEY_SET1_WWWBACK_BREAK}, //548
	{549, KEY_SET1_WWWFORWARD_BREAK}, //549
	{550, KEY_SET1_WWWSTOP_BREAK}, //550
	{551, KEY_SET1_WWWREFRESH_BREAK}, //551
	{554, KEY_SET1_WWWFAVE_BREAK}, //554
	{NULL, NULL}
};

const EXTCHARLOOKUP HID0CtoSET2_Make[] = {
	{181, KEY_SET2_MEDIANEXT_MAKE}, //181
	{182, KEY_SET2_MEDIAPREV_MAKE}, //182
	{183, KEY_SET2_MEDIASTOP_MAKE}, //183
	{205, KEY_SET2_MEDIANPLAY_MAKE}, //205
	{226, KEY_SET2_MEDIAMUTE_MAKE}, //226
	{233, KEY_SET2_VOLUP_MAKE}, //233
	{234, KEY_SET2_VOLDN_MAKE}, //234
	{387, KEY_SET2_MEDSELECT_MAKE}, //387
	{394, KEY_SET2_APPMAIL_MAKE}, //394
	{402, KEY_SET2_APPCALC_MAKE}, //402
	{404, KEY_SET2_APPMYCOMP_MAKE}, //404
	{545, KEY_SET2_WWWSEARCH_MAKE}, //545
	{547, KEY_SET2_WWWHOME_MAKE}, //547
	{548, KEY_SET2_WWWBACK_MAKE}, //548
	{549, KEY_SET2_WWWFORWARD_MAKE}, //549
	{550, KEY_SET2_WWWSTOP_MAKE}, //550
	{551, KEY_SET2_WWWREFRESH_MAKE}, //551
	{554, KEY_SET2_WWWFAVE_MAKE}, //554
	{NULL, NULL}
};

const EXTCHARLOOKUP HID0CtoSET2_Break[] = {
	{181, KEY_SET2_MEDIANEXT_BREAK}, //181
	{182, KEY_SET2_MEDIAPREV_BREAK}, //182
	{183, KEY_SET2_MEDIASTOP_BREAK}, //183
	{205, KEY_SET2_MEDIANPLAY_BREAK}, //205
	{226, KEY_SET2_MEDIAMUTE_BREAK}, //226
	{233, KEY_SET2_VOLUP_BREAK}, //233
	{234, KEY_SET2_VOLDN_BREAK}, //234
	{387, KEY_SET2_MEDSELECT_BREAK}, //387
	{394, KEY_SET2_APPMAIL_BREAK}, //394
	{402, KEY_SET2_APPCALC_BREAK}, //402
	{404, KEY_SET2_APPMYCOMP_BREAK}, //404
	{545, KEY_SET2_WWWSEARCH_BREAK}, //545
	{547, KEY_SET2_WWWHOME_BREAK}, //547
	{548, KEY_SET2_WWWBACK_BREAK}, //548
	{549, KEY_SET2_WWWFORWARD_BREAK}, //549
	{550, KEY_SET2_WWWSTOP_BREAK}, //550
	{551, KEY_SET2_WWWREFRESH_BREAK}, //551
	{554, KEY_SET2_WWWFAVE_BREAK}, //554
	{NULL, NULL}
};

