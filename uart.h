#ifndef _UART_H_
#define _UART_H_

extern void CH559UART0Alter(void);

extern void InitUART0(void);

extern UINT8 CH559UART0RcvByte(void);

extern void SetUart0Sent(void);

extern void CH559UART0SendByte(UINT8 SendDat);

extern void CH559UART0SendData(UINT8 *pData, UINT8 len);

void UART1RegCfgValue(void);
void ResetUART1(void);
void CH559UART1Init(UINT8 DIV,UINT8 mode,UINT8 pin,UINT32 bps,UINT8 databits);
UINT8 CH559UART1RcvByte(void);
void CH559UART1Rcv(PUINT8 buf,UINT8 len);
void CH559UART1SendByte(UINT8 SendDat);
void CH559UART1SendStr(PUINT8 SendStr);

extern UINT8 CH559UART1_FIFO_CNT;
#endif

