
#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
* Function Name  : CH559GPIODrivCap(UINT8 Port,UINT8 Cap)
* Description    : 端口0、1、2、3驱动能力设置
* Input          : UINT8 Port端口选择(0、1、2、3)
                   UINT8 Cap驱动能力选择((0)5mA、(1)20mA(注意:P1口是10mA))
* Output         : None
* Return         : SUCCESS成功
                   FAIL失败
*******************************************************************************/
extern UINT8 CH559GPIODrivCap(UINT8 Port,UINT8 Cap);


/*******************************************************************************
* Function Name  : CH559GPIOModeSelt(UINT8 Port,UINT8 Mode,UINT8 PinNum)
* Description    : 端口0、1、2、3引脚模式设置
* Input          : UINT8 Port端口选择(0、1、2、3)
                   UINT8 Cap驱动方式选择(bPn_OC & Pn_DIR & Pn_PU)
                   0(000)：仅输入，无上拉；
                   1(001)：仅输入，带上拉；
                   2(01x)：推挽输出，高低电平强驱动；
                   3(100)：开漏输出，无上拉，支持输入；
                   4(110)：开漏输出，无上拉,当转变输出由低到高时，仅驱动2个时钟的高电平
                   5(101)：准双向(标准51模式)，开漏输出，带上拉
                   6(111)：准双向(标准51模式)，开漏输出，带上拉，当转变输出由低到高时，仅驱动2个时钟的高电平
                   UINT8 PinNum(引脚选择0-7)
* Output         : None
* Return         : SUCCESS成功
                   FAIL失败
*******************************************************************************/
extern UINT8 CH559GPIOModeSelt(UINT8 Port,UINT8 Mode,UINT8 PinNum);


/*******************************************************************************
* Function Name  : CH559P4Mode()
* Description    : CH559的P4端口初始化，P4默认是输入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void CH559P4Mode();


/*******************************************************************************
* Function Name  : CH559GPIOInterruptInit()
* Description    : CH559GPIO中断初始化，其他引脚如P5.5\P1.4\P0.3\P5.7\P4.1\RXD0设置同理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void CH559GPIOInterruptInit();

#endif

