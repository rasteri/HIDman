
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// Keyboard Ouput Modes (PS2, XT, AMSTRAD)
#define MODE_PS2 0
#define MODE_XT 1
#define MODE_AMSTRAD 2

typedef struct Settings {

    // will be set to 0x54178008 to make sure flash is intact
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

} Settings;

#define FlashSettings ((__code Settings*)0xF000)

extern __xdata Settings HMSettings;

void InitSettings(bool SafeMode);

uint8_t SyncSettings(void);

#endif

