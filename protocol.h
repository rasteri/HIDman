#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__


void HandleReceived(uint8_t port);
void SendHIDPS2(unsigned short length, unsigned char type, unsigned char __xdata *msgbuffer);

#endif //__PROTOCOL_H__