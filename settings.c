#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "ch559.h"
#include "util.h"
#include "usbhost.h"
#include "uart.h"
#include "ps2.h"
#include "data.h"
#include "ps2protocol.h"
#include "mouse.h"
#include "dataflash.h"

#include "settings.h"

__xdata Settings HMSettings;
uint8_t* arse = ((uint8_t *)FlashSettings);
uint8_t SyncSettings() {
    
    if(EraseDataFlash(0xF000) == 0){
        if (WriteDataFlash(0xF000, (uint8_t *)&HMSettings, sizeof(Settings)) == 0)
            return 0;
        else DEBUG_OUT("Cant Write\n");
    }else DEBUG_OUT("Cant Erase\n");
    return 1;
}

void InitSettings(){
    // magic value not present, initialize flash data
    if (FlashSettings->Magic != 0x54178008){

        DEBUG_OUT("Magic Missing\n");

        memset(&HMSettings, 0x00, sizeof(Settings));
        HMSettings.Magic = 0x54178008;

        HMSettings.Intellimouse = 1;
        
        if (SyncSettings()){
            DEBUG_OUT("Writin failed\n");

        }
    }
    memcpy(&HMSettings, FlashSettings, sizeof(Settings));

    if (HMSettings.Magic != 0x54178008){
        // failed, do something bad
        DEBUG_OUT("Initing settings failed\n");
    }
}

