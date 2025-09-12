#pragma once
void TypematicDefaults(void);
extern __xdata volatile int16_t RepeatState;

extern __xdata volatile uint8_t RepeatKey;
extern __xdata int16_t RepeatDelay;
extern __xdata int16_t RepeatRate;

extern __xdata uint8_t LEDDelayMs;

extern __code int16_t DelayConv[];
extern __code int16_t RateConv[];
uint32_t SegExtractValue(__xdata HID_SEG *currSeg, __xdata uint8_t *data);