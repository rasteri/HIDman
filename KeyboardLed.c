#include "Type.h"
#include "mcu.h"

#include "KeyboardLed.h"

static UINT8 keyboardLed = 0x00;

void SetKeyboardLedStatus(UINT8 led)
{
	HAL_CRITICAL_STATEMENT(keyboardLed = led);
}

UINT8 GetKeyboardLedStatus(void)
{
	UINT8 led;
	HAL_CRITICAL_STATEMENT(led = keyboardLed);

	return led;
}

