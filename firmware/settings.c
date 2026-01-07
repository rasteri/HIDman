#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "ch559.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "mouse.h"
#include "dataflash.h"
#include "system.h"
#include "settings.h"
#include "pwm.h"

__xdata Settings HMSettings;

uint8_t* arse = ((uint8_t *)FlashSettings);

uint8_t SyncSettings(void) {
    
    if(EraseDataFlash(0xF000) == 0){
        if (WriteDataFlash(0xF000, (uint8_t *)&HMSettings, sizeof(Settings)) == 0)
            return 0;
        else DEBUGOUT("Cant Write\n");
    }else DEBUGOUT("Cant Erase\n");
    return 1;
}

void InitSettings(bool SafeMode) {

    // magic value not present (or we're in safe mode), initialize flash data
    if (SafeMode || FlashSettings->Magic != 0x54178008) {

        SetPWM1Dat(0x40);
        SetPWM2Dat(0x00);
        T3_FIFO_L = 0;
        T3_FIFO_H = 0;
        LEDDelayMs = 255;

        DEBUGOUT("Magic Missing\n");

        memset(&HMSettings, 0x00, sizeof(Settings));
        HMSettings.Magic = 0x54178008;

        //if (!SafeMode) HMSettings.Intellimouse = 1;
        HMSettings.Intellimouse = 1;
        //HMSettings.EnableAUXPS2 = 1;
        HMSettings.SerialDebugOutput = 1;
        //HMSettings.MouseReportMode = 1;
        if (SyncSettings()) {
            DEBUGOUT("Writin failed\n");
        }
    }
    memcpy(&HMSettings, FlashSettings, sizeof(Settings));

    if (HMSettings.Magic != 0x54178008){
        // failed, do something bad
        DEBUGOUT("Initing settings failed\n");
    }
}

