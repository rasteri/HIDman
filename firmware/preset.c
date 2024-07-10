/*
    preset.c

    Handles game controller mapping presets

*/
#include <stdio.h>
#include <string.h>
#include "andyalloc.h"
#include "data.h"
#include "defs.h"
#include "settings.h"
#include "system.h"

__xdata JoyPreset *JoyPresets = NULL;
JoyPreset *LastPreset = NULL;

void LoadPreset(JoyPreset *Preset)
{
    uint8_t k = 0;

    while (Preset[k].InputType != MAP_TYPE_NONE)
    {
        if (JoyPresets == NULL)
        {
            JoyPresets = (JoyPreset *)andyalloc(sizeof(JoyPreset));
            LastPreset = JoyPresets;
        }
        else
        {
            LastPreset->next = (JoyPreset *)andyalloc(sizeof(JoyPreset));
            LastPreset = LastPreset->next;
        }

        memcpy(LastPreset, &Preset[k], sizeof(JoyPreset));
        LastPreset->next = NULL;
        k++;
    }
}

void DumpPresets(void){
    JoyPreset *currPreset = JoyPresets;

	while (currPreset != NULL)
	{
        DEBUG_OUT("%x ", currPreset->InputUsage);
		currPreset = currPreset->next;
	}
}

void InitPresets(void) {
    // Load the base presets
    LoadPreset(DefaultJoyMaps);

    // Some buttons get mapped differently depending on if
    // game controllers are set to emulate mice
    if (FlashSettings->GameControllerAsMouse)
        LoadPreset(ConfigGameMouse);
    else
        LoadPreset(ConfigGameNoMouse);

    //DumpPresets();
}