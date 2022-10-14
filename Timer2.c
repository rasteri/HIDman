
#include "Type.h"
#include "CH559.h"
#include "Timer2.h"

/*******************************************************************************
* Function Name  : mTimer2Setup(UINT8 T2Out)
* Description    : CH559定时2初始化
* Input          : UINT8 T2Out,是否允许T2输出时钟
                   0：不允许输出
                   1：允许输出  
* Output         : None
* Return         : None
*******************************************************************************/
void mTimer2Setup(UINT8 T2Out)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 0;                                                                //启动自动重载定时器功能
    if(T2Out)
    {
	      T2MOD |= T2OE;                                                        //是否允许T2输出时钟,如果允许时钟=1/2定时器2溢出率
    }
    else
    {
	      T2MOD &= ~T2OE;
    }
}

/*******************************************************************************
* Function Name  : mTimer2Init(UINT16 Tim)
* Description    : CH559 T2定时器赋初值                   
* Input          : UINT16 Tim,定时器初值
* Output         : None
* Return         : None
*******************************************************************************/
void mTimer2Init(UINT16 Tim)
{
    UINT16 tmp;
    tmp = 65536 - Tim;
    RCAP2L = TL2 = tmp & 0xff;
    RCAP2H = TH2 = (tmp >> 8) & 0xff;
}

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
void T2exCaptureSetup(UINT8 mode)
{
    C_T2  = 0;
    EXEN2 = 1; 
    CP_RL2 = 1;                                                                //启动T2ex的捕捉功能
    T2MOD |= mode << 2;                                                        //边沿捕捉模式选择
}

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
void T2CaptureSetup(UINT8 mode)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 1;
    C_T2 = 0;
    T2MOD &= ~T2OE;                                                            //使能T2引脚捕捉功能
    T2MOD |= (mode << 2) | bT2_CAP1_EN;                                        //边沿捕捉模式选择
}

