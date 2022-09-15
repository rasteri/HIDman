#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

void HandleRepeats();
void HandleReceived(uint8_t port);
void SendHIDPS2(unsigned short length, __xdata unsigned char devnum, unsigned char type, unsigned char __xdata *msgbuffer);
void RepeatTimer();

bool ParseReport(HID_REPORT_DESC *desc, uint32_t len, uint8_t *report);

#define SetRepeatState(st) TR0 = 0; RepeatState = st; TR0 = 1;

#endif //__PROTOCOL_H__