
#ifndef _TASK_H_
#define _TASK_H_

extern void InitSystem(void);

extern void ProcessUsbHostPort(void);

extern void ProcessPs2Port(void);

extern void ProcessIpPs2Port(void);

#define mTimer2RunCTL( SS )    {TR2 = SS ? START : STOP;}
#define mTimer2ClkFsys( )      {T2MOD |= (bTMR_CLK | bT2_CLK);C_T2=0;}         //��ʱ��,ʱ��=Fsys
#define mTimer2Clk4DivFsys( )  {T2MOD &= ~bTMR_CLK;T2MOD |=  bT2_CLK;C_T2 = 0;}//��ʱ��,ʱ��=Fsys/4
#define mTimer2Clk12DivFsys( ) {T2MOD &= ~(/*bTMR_CLK | */bT2_CLK);C_T2 = 0;}      //��ʱ��,ʱ��=Fsys/12
#define mTimer2CountClk( )     {C_T2 = 1;}                                     //������,T2���ŵ��½�����Ч

#endif

