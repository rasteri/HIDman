#ifndef	__SYSTEM_H__
#define __SYSTEM_H__


//#define	 FREQ_SYS	48000000ul	  //ϵͳ��Ƶ48MHz

#ifndef  BUAD_RATE
#ifdef DEBUG
#define  BUAD_RATE  115200ul
#else
#define  BUAD_RATE  115200ul
#endif
#endif

void CfgFsys(void);                        //CH559ʱ��ѡ�������
void mDelayuS(UINT16 n);              // ��uSΪ��λ��ʱ
void mDelaymS(UINT16 n);              // ��mSΪ��λ��ʱ

#include <stdio.h>


void initClock(void);
unsigned char UART0Receive(void);
void UART0Send(unsigned char b);
int putcharserial(int c);

#define st(x)      do { x } while (__LINE__ == -1)

#define HAL_ENABLE_INTERRUPTS()         st( EA = 1; )
#define HAL_DISABLE_INTERRUPTS()        st( EA = 0; )
#define HAL_INTERRUPTS_ARE_ENABLED()    (EA)

typedef unsigned char halIntState_t;
#define HAL_ENTER_CRITICAL_SECTION(x)   st( x = EA;  HAL_DISABLE_INTERRUPTS(); )
#define HAL_EXIT_CRITICAL_SECTION(x)    st( EA = x; )
#define HAL_CRITICAL_STATEMENT(x)       st( halIntState_t _s; HAL_ENTER_CRITICAL_SECTION(_s); x; HAL_EXIT_CRITICAL_SECTION(_s); )


typedef void(* __data FunctionReference)(void);
extern FunctionReference runBootloader;

void GPIOInit(void);
void ClockInit(void);

extern __xdata volatile uint16_t SoftWatchdog;
extern __xdata volatile bool OutputsEnabled;
#define DEBUGOUT(...) { printf(__VA_ARGS__);}
#endif

