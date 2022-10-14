
#ifndef _TIMER2_H_
#define _TIMER2_H_

//CH559 Timer2时钟选择   
//bTMR_CLK同时影响Timer0&1&2,使用时要注意                                                       
#define mTimer2ClkFsys( )      {T2MOD |= (bTMR_CLK | bT2_CLK);C_T2=0;}         //定时器,时钟=Fsys
#define mTimer2Clk4DivFsys( )  {T2MOD &= ~bTMR_CLK;T2MOD |=  bT2_CLK;C_T2 = 0;}//定时器,时钟=Fsys/4
#define mTimer2Clk12DivFsys( ) {T2MOD &= ~(/*bTMR_CLK | */bT2_CLK);C_T2 = 0;}      //定时器,时钟=Fsys/12
#define mTimer2CountClk( )     {C_T2 = 1;}                                     //计数器,T2引脚的下降沿有效

//CH559 Timer2 开始(SS=1)/结束(SS=0)
#define mTimer2RunCTL( SS )    {TR2 = SS ? START : STOP;}


/*******************************************************************************
* Function Name  : mTimer2Setup(UINT8 T2Out)
* Description    : CH559定时2初始化
* Input          : UINT8 T2Out,是否允许T2输出时钟
                   0：不允许输出
                   1：允许输出  
* Output         : None
* Return         : None
*******************************************************************************/
extern void mTimer2Setup(UINT8 T2Out);


/*******************************************************************************
* Function Name  : mTimer2Init(UINT16 Tim)
* Description    : CH559 T2定时器赋初值                   
* Input          : UINT16 Tim,定时器初值
* Output         : None
* Return         : None
*******************************************************************************/
extern void mTimer2Init(UINT16 Tim);


/*******************************************************************************
* Function Name  : T2exCaptureSetup(UINT8 mode)
* Description    : CH559定时计数器2 T2EX引脚捕捉功能初始化
                   UINT8 mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void T2exCaptureSetup(UINT8 mode);


/*******************************************************************************
* Function Name  : T2CaptureSetup(UINT8 mode)
* Description    : CH559定时计数器2 T2引脚捕捉功能初始化T2
                   UINT8 mode,边沿捕捉模式选择
                   0:T2ex从下降沿到下一个下降沿
                   1:T2ex任意边沿之间
                   3:T2ex从上升沿到下一个上升沿
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void T2CaptureSetup(UINT8 mode);

#endif

