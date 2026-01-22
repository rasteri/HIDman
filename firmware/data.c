/*
    data.c
    
    Various tables - 
    Joystick mapping presets, Keyboard scancodes, HID->PS2 mappings, etc
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ps2.h"
#include "defs.h"
#include "usbhidkeys.h"
#include "data.h"
#include "scancode.h"

// configuration for when game controller mouse emulation is enabled
// buttons 5 and 6 mapped to mouse buttons
__code JoyPreset ConfigGameMouse[] = {
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        5,                        // Input Usage
        MAP_MOUSE,                // Output Channel
        MAP_MOUSE_BUTTON2,        // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        6,                        // Input Usage
        MAP_MOUSE,                // Output Channel
        MAP_MOUSE_BUTTON1,        // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_Z,            // Usage
        MAP_MOUSE,                 // Output Channel
        MAP_MOUSE_X,               // Output Control
        MAP_TYPE_SCALE,            // InputType
        INPUT_PARAM_UNSIGNED_SCALEDOWN                          // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_Rz,           // Usage
        MAP_MOUSE,                 // Output Channel
        MAP_MOUSE_Y,               // Output Control
        MAP_TYPE_SCALE,            // InputType
        INPUT_PARAM_UNSIGNED_SCALEDOWN                          // Input Param
    },
    // null to signify end
    {
        0, // Number
        0, // Input Usage Page
        0, // Input Usage
        0, // Output Channel
        0, // Output Control
        0, // InputType
        0  // Input Param
    }
};

// game controller mouse emulation NOT enabled
// buttons 5 and 6 should just be mapped to keys
__code JoyPreset ConfigGameNoMouse[] = {
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        5,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_E,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        6,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_F,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    // null to signify end
    {
        0, // Number
        0, // Input Usage Page
        0, // Input Usage
        0, // Output Channel
        0, // Output Control
        0, // InputType
        0  // Input Param
    },
};



__code JoyPreset DefaultJoyMaps[] = {

    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        1,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_A,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        2,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_B,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        3,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_C,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        4,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_D,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        7,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_G,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        8,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_H,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        9,                        // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_ESC,                  // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        10,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_ENTER,                // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        11,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_I,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        12,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_J,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        13,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_K,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        14,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_L,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        15,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_M,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                        // Number
        REPORT_USAGE_PAGE_BUTTON, // Input Usage Page
        16,                       // Input Usage
        MAP_KEYBOARD,             // Output Channel
        KEY_N,                    // Output Control
        MAP_TYPE_THRESHOLD_ABOVE, // InputType
        0                         // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_X,            // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_RIGHT,                 // Output Control
        MAP_TYPE_THRESHOLD_ABOVE,  // InputType
        192                        // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_X,            // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_LEFT,                  // Output Control
        MAP_TYPE_THRESHOLD_BELOW,  // InputType
        64                         // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_Y,            // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_DOWN,                  // Output Control
        MAP_TYPE_THRESHOLD_ABOVE,  // InputType
        192                        // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_Y,            // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_UP,                    // Output Control
        MAP_TYPE_THRESHOLD_BELOW,  // InputType
        64                         // Input Param
    },
    // 0 on hat switch, just press up
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_UP,                    // Output Control
        MAP_TYPE_EQUAL,            // InputType
        0                          // Input Param
    },
    // 1 on hatswitch, press up and right
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_UP,                    // Output Control
        MAP_TYPE_EQUAL,            // InputType
        1                          // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_RIGHT,                 // Output Control
        MAP_TYPE_EQUAL,            // InputType
        1                          // Input Param
    },
    // 2 on hatswitch, press right
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_RIGHT,                 // Output Control
        MAP_TYPE_EQUAL,            // InputType
        2                          // Input Param
    },
    // 3 on hat, press right and down
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_RIGHT,                 // Output Control
        MAP_TYPE_EQUAL,            // InputType
        3                          // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_DOWN,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        3                          // Input Param
    },
    // 4 on hat, press down
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_DOWN,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        4                          // Input Param
    },
    // 5 on hat, press down and left
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_DOWN,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        5                          // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_LEFT,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        5                          // Input Param
    },
    // 6 on hat, press left
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_LEFT,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        6                          // Input Param
    },
    // 7 on hat, press left and up
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_LEFT,                  // Output Control
        MAP_TYPE_EQUAL,            // InputType
        7                          // Input Param
    },
    {
        1,                         // Number
        REPORT_USAGE_PAGE_GENERIC, // Usage Page
        REPORT_USAGE_HATSWITCH,    // Usage
        MAP_KEYBOARD,              // Output Channel
        KEY_UP,                    // Output Control
        MAP_TYPE_EQUAL,            // InputType
        7                          // Input Param
    },
    // null to signify end
    {
        0, // Number
        0, // Input Usage Page
        0, // Input Usage
        0, // Output Channel
        0, // Output Control
        0, // InputType
        0  // Input Param
    },

};



__code uint8_t ASCIItoHID[] = {
    0, // 0
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
    0x1E,
    0,
    0x20,
    0x21,
    0x22,
    0x24,
    0x34,
    0x26, // 40
    0x27,
    0x25,
    0x2E,
    0x36,
    0x2D,
    0x37,
    0x38,
    0x27,
    0x1E,
    0x1F, // 50
    0x20,
    0x21,
    0x22,
    0x23,
    0x24,
    0x25,
    0x26,
    0x33,
    0x33,
    0x36, // 60
    0x2E,
    0x37,
    0x38,
    0x1F,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09, // 70
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
    0x10,
    0x11,
    0x12,
    0x13, // 80
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1A,
    0x1B,
    0x1C,
    0x1D, // 90
    0x2F,
    0,
    0x30,
    0x23,
    0,
    0,
    0x04,
    0x05,
    0x06,
    0x07, // 100
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
    0x10,
    0x11, // 110
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1A,
    0x1B, // 120
    0x1C,
    0x1D,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // 130
};

__code uint8_t QWERTYtoOther[][131] = {
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        '+',
        ',',
        '-',
        '.',
        0,
        '0',
        '1',
        '2', // 50
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'A',
        'B',
        'C',
        'D',
        'E',
        'F', // 70
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P', // 80
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Y',
        'Z', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'a',
        'b',
        'c',
        'd', // 100
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n', // 110
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x', // 120
        'y',
        'z',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        '+',
        'm',
        '6',
        '<',
        0,
        ')',
        '!',
        '@', // 50
        '#',
        '$',
        '%',
        '^',
        '&',
        '*',
        '(',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'Q',
        'B',
        'C',
        'D',
        'E',
        'F', // 70
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        ':',
        'N',
        'O',
        'P', // 80
        'A',
        'R',
        'S',
        'T',
        'U',
        'V',
        'Z',
        'X',
        'Y',
        'W', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'q',
        'b',
        'c',
        'd', // 100
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        ';',
        'n', // 110
        'o',
        'p',
        'a',
        'r',
        's',
        't',
        'u',
        'v',
        'z',
        'x', // 120
        'y',
        'w',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        ']',
        ',',
        '/',
        '.',
        0,
        '0',
        '1',
        '2', // 50
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'A',
        'B',
        'C',
        'D',
        'E',
        'F', // 70
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P', // 80
        'Q',
        'R',
        'S',
        'T',
        'U',
        'V',
        'W',
        'X',
        'Z',
        'Y', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'a',
        'b',
        'c',
        'd', // 100
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n', // 110
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x', // 120
        'y',
        'z',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        ']',
        'w',
        '\'',
        'e',
        0,
        '0',
        '1',
        '2', // 50
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'A',
        'N',
        'I',
        'H',
        'D',
        'Y', // 70
        'U',
        'J',
        'G',
        'C',
        'V',
        'P',
        'M',
        'L',
        'S',
        'R', // 80
        'X',
        'O',
        ':',
        'K',
        'F',
        '>',
        '<',
        'B',
        'T',
        '?', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'a',
        'n',
        'i',
        'h', // 100
        'd',
        'y',
        'u',
        'j',
        'g',
        'c',
        'v',
        'p',
        'm',
        'l', // 110
        's',
        'r',
        'x',
        'o',
        ';',
        'k',
        'f',
        '.',
        ',',
        'b', // 120
        't',
        '/',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        ']',
        ',',
        'a',
        'o',
        0,
        '.',
        '=',
        '-', // 50
        '0',
        '9',
        '[',
        'p',
        '\'',
        ';',
        '/',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'K',
        'E',
        'D',
        'F',
        'J',
        '%', // 70
        'C',
        'H',
        'M',
        '*',
        'S',
        '&',
        '^',
        'N',
        'I',
        '$', // 80
        'W',
        'Y',
        'U',
        'G',
        'T',
        'V',
        'B',
        'X',
        'R',
        'L', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'k',
        'e',
        'd',
        'f', // 100
        'j',
        '5',
        'c',
        'h',
        'm',
        '8',
        's',
        '7',
        '6',
        'n', // 110
        'i',
        '4',
        'w',
        'y',
        'u',
        'g',
        't',
        'v',
        'b',
        'x', // 120
        'r',
        'l',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        ']',
        'v',
        '\'',
        'r',
        0,
        'x',
        '1',
        '2', // 50
        '3',
        '4',
        's',
        'a',
        'w',
        'q',
        'z',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'F',
        'P',
        'L',
        'K',
        'G',
        '*', // 70
        '>',
        'H',
        'B',
        '%',
        ':',
        '^',
        '7',
        'N',
        'T',
        '(', // 80
        'E',
        'Y',
        'U',
        'J',
        'I',
        '<',
        'M',
        'C',
        'O',
        'D', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'f',
        'p',
        'l',
        'k', // 100
        'g',
        '8',
        '.',
        'h',
        'b',
        '5',
        ';',
        '6',
        '7',
        'n', // 110
        't',
        '9',
        'e',
        'y',
        'u',
        'j',
        'i',
        ',',
        'm',
        'c', // 120
        'o',
        'd',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        '+',
        ',',
        '-',
        '.',
        0,
        '0',
        '1',
        '2', // 50
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'A',
        'B',
        'C',
        'G',
        'K',
        'E', // 70
        'T',
        'H',
        'L',
        'Y',
        'N',
        'U',
        'M',
        'J',
        ':',
        'P', // 80
        'Q',
        'S',
        'D',
        'F',
        'I',
        'V',
        'W',
        'X',
        'O',
        'Z', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'a',
        'b',
        'c',
        'g', // 100
        'k',
        'e',
        't',
        'h',
        'l',
        'y',
        'n',
        'u',
        'm',
        'j', // 110
        ';',
        'p',
        'q',
        's',
        'd',
        'f',
        'i',
        'v',
        'w',
        'x', // 120
        'o',
        'z',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    },
    {
        0, // 0
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x0A, //ENTER // 10
        0,
        0,
        0x0A, //ENTER
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
        0x20, //SPACE
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
        '+',
        ',',
        '-',
        '.',
        0,
        '0',
        '1',
        '2', // 50
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        0,
        0,
        0, // 60
        0,
        0,
        0,
        0,
        'A',
        'T',
        'V',
        'W',
        'K',
        'U', // 70
        'G',
        'D',
        ':',
        'Y',
        'N',
        'M',
        'C',
        'J',
        'L',
        'O', // 80
        'Q',
        'E',
        'S',
        'F',
        'I',
        'B',
        'R',
        'X',
        'H',
        'Z', // 90
        0,
        0,
        0,
        0,
        0,
        0,
        'a',
        't',
        'v',
        'w', // 100
        'k',
        'u',
        'g',
        'd',
        ';',
        'y',
        'n',
        'm',
        'c',
        'j', // 110
        'l',
        'o',
        'q',
        'e',
        's',
        'f',
        'i',
        'b',
        'r',
        'x', // 120
        'h',
        'z',
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0, // 130
    }
};

__xdata uint8_t StandardKeyboardDescriptor[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06, // Usage (Keyboard)
    0xA1, 0x01, // Collection (Application)
    0x05, 0x07, //   Usage Page (Kbrd/Keypad)
    0x19, 0xE0, //   Usage Minimum (0xE0)
    0x29, 0xE7, //   Usage Maximum (0xE7)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x01, //   Logical Maximum (1)
    0x75, 0x01, //   Report Size (1)
    0x95, 0x08, //   Report Count (8)
    0x81, 0x02, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01, //   Report Count (1)
    0x75, 0x08, //   Report Size (8)
    0x81, 0x01, //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05, //   Report Count (5)
    0x75, 0x01, //   Report Size (1)
    0x05, 0x08, //   Usage Page (LEDs)
    0x19, 0x01, //   Usage Minimum (Num Lock)
    0x29, 0x05, //   Usage Maximum (Kana)
    0x91, 0x02, //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01, //   Report Count (1)
    0x75, 0x03, //   Report Size (3)
    0x91, 0x01, //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x06, //   Report Count (6)
    0x75, 0x08, //   Report Size (8)
    0x15, 0x00, //   Logical Minimum (0)
    0x25, 0x65, //   Logical Maximum (101)
    0x05, 0x07, //   Usage Page (Kbrd/Keypad)
    0x19, 0x00, //   Usage Minimum (0x00)
    0x29, 0x65, //   Usage Maximum (0x65)
    0x81, 0x00, //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,       // End Collection

    // 63 bytes

};

__xdata uint8_t StandardMouseDescriptor[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x09, 0x01, //   Usage (Pointer)
    0xA1, 0x00, //   Collection (Physical)
    0x05, 0x09, //     Usage Page (Button)
    0x19, 0x01, //     Usage Minimum (0x01)
    0x29, 0x05, //     Usage Maximum (0x05)
    0x15, 0x00, //     Logical Minimum (0)
    0x25, 0x01, //     Logical Maximum (1)
    0x95, 0x05, //     Report Count (5)
    0x75, 0x01, //     Report Size (1)
    0x81, 0x02, //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x01, //     Report Count (1)
    0x75, 0x03, //     Report Size (3)
    0x81, 0x01, //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01, //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30, //     Usage (X)
    0x09, 0x31, //     Usage (Y)
    0x15, 0x81, //     Logical Minimum (-127)
    0x25, 0x7F, //     Logical Maximum (127)
    0x75, 0x08, //     Report Size (8)
    0x95, 0x02, //     Report Count (2)
    0x81, 0x06, //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,       //   End Collection
    0xC0,       // End Collection

    // 50 bytes

};
