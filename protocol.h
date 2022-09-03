#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

void HandleRepeats();
void HandleReceived(uint8_t port);
void SendHIDPS2(unsigned short length, __xdata unsigned char devnum, unsigned char type, unsigned char __xdata *msgbuffer);
void RepeatTimer();

#define SetRepeatState(st) TR0 = 0; repeatState = st; TR0 = 1;

#endif //__PROTOCOL_H__