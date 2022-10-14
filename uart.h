#ifndef _UART_H_
#define _UART_H_

extern void CH559UART0Alter(void);

extern void InitUART0(void);

extern UINT8 CH559UART0RcvByte(void);

extern void SetUart0Sent(void);

extern void CH559UART0SendByte(UINT8 SendDat);

extern void CH559UART0SendData(UINT8 *pData, UINT8 len);

#endif

