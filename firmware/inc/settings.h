
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// Keyboard Ouput Modes (PS2, XT, AMSTRAD)
#define MODE_PS2 0
#define MODE_XT 1
#define MODE_AMSTRAD 2

// Keyboard Layouts (QWERTY, AZERTY, QWERTZ, DVORAK, DVORAK Left Hand, DVORAK Right Hand, COLEMAK, WORKMAN)
#define LAYOUT_QWERTY 0
#define LAYOUT_AZERTY 1
#define LAYOUT_QWERTZ 2
#define LAYOUT_DVORAK 3
#define LAYOUT_DVORAKLH 4
#define LAYOUT_DVORAKRH 5
#define LAYOUT_COLEMAK 6
#define LAYOUT_WORKMAN 7

typedef struct Settings {

    // will be set to 0x54178007 to make sure flash is intact
    uint32_t Magic;

    // Set all USB keyboards to run in Report Mode rather than Boot Mode
    bool KeyboardReportMode;

    // Set all USB mice to run in Report Mode rather than Boot Mode
    bool MouseReportMode;

    // Output mouse signals on the keyboard port's auxilliary pins (and vice versa)
    bool EnableAUXPS2;

    // Emulate 3rd PS/2 button + Wheel
    bool Intellimouse;

    // Limit XT keyboards to 81 keys only
    bool XT83Keys;

    // Use game controller to control mouse
    bool GameControllerAsMouse;

    // Output debug info to serial port
    bool SerialDebugOutput;

    // blocks garbage packets that broken USB devices emit
    bool USBFilter;

    // What type of computer the keyboard is plugged into (i.e. PS2 or XT)
    uint8_t KeyboardMode;

    // Whether or not the menu should be rate limited
    uint8_t MenuRateLimit;

    // Keyboard layout to be used when interacting with internal menu
    uint8_t KeyboardLayout;

} Settings;

#define FlashSettings ((__code Settings*)0xF000)

extern __xdata Settings HMSettings;

void InitSettings(bool SafeMode);

uint8_t SyncSettings(void);

#endif

