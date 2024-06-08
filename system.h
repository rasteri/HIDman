#ifndef	__SYSTEM_H__
#define __SYSTEM_H__

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS  1
#endif
#ifndef  FAIL
#define  FAIL    0
#endif

//���嶨ʱ����ʼ
#ifndef  START
#define  START  1
#endif
#ifndef  STOP
#define  STOP    0
#endif

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
#define ANDYS_DEBUG_OUT(...) printf(__VA_ARGS__);
#if 0
#define DEBUG_OUT(...) printf(__VA_ARGS__);
#else
#define DEBUG_OUT(...) (void)0;
#endif

void initClock(void);
unsigned char UART0Receive(void);
void UART0Send(unsigned char b);


#define DYNAMIC_MEMORY_SIZE 0x400
extern unsigned char __xdata dynamic_memory_pool[DYNAMIC_MEMORY_SIZE];

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

#endif

