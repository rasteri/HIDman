
#include "Type.h"
#include "CH559.h"
#include "System.h"
#include "GPIO.h"

/*******************************************************************************
* Function Name  : CH559GPIODrivCap(UINT8 Port,UINT8 Cap)
* Description    : 端口0、1、2、3驱动能力设置
* Input          : UINT8 Port端口选择(0、1、2、3)
                   UINT8 Cap驱动能力选择((0)5mA、(1)20mA(注意:P1口是10mA))
* Output         : None
* Return         : SUCCESS成功
                   FAIL失败
*******************************************************************************/
UINT8 CH559GPIODrivCap(UINT8 Port,UINT8 Cap)
{
    if (Port >= 4)
    {
    	return FAIL;
    }
    
    if (Cap == 0)
    {                                                               //驱动电流最大5mA
        PORT_CFG &= ~(bP0_DRV << Port);
    }
    else
    {		
        PORT_CFG |= (bP0_DRV << Port);                                             //驱动电流最大20mA
    }

    return SUCCESS;
}

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
UINT8 CH559GPIOModeSelt(UINT8 Port,UINT8 Mode,UINT8 PinNum)
{
    UINT8 Pn_DIR,Pn_PU;
    if (Port >= 4)
    {
    	return FAIL;
    }

    switch (Port)
    {
    case 0:
        Pn_DIR = P0_DIR;
		Pn_PU = P0_PU;
		
        break;

    case 1:
        Pn_DIR = P1_DIR;
		Pn_PU = P1_PU;
		
        break;

    case 2:
        Pn_DIR = P2_DIR;
		Pn_PU = P2_PU;
		
        break;

    case 3:
        Pn_DIR = P3_DIR;
		Pn_PU = P3_PU;
		
        break;
    }
    
    switch (Mode)
    {
    case 0:                              //仅输入，无上拉
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 1:                              //仅输入，带上拉
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU |= 1<<PinNum;
    	
    	break;
    	
    case 2:                              //推挽输出，高低电平强驱动
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR |= (1<<PinNum);
    	
    	break;
    	
    case 3:                              //开漏输出，无上拉，支持输入
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 4:                              //开漏输出，无上拉,当转变输出由低到高时，仅驱动2个时钟的高电平
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR |= 1<<PinNum;
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 5:                              //弱准双向(标准51模式)，开漏输出，带上拉
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU |= 1<<PinNum;
    	
    	break;
    	
    case 6:                              //准双向(标准51模式)，开漏输出，带上拉，当转变输出由低到高时，仅驱动2个时钟的高电平
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR |= 1<<PinNum;
    	Pn_PU |= 1<<PinNum;
    	
    	break;
    	
    default:
    	break;
    }

    switch (Port)
    {
    case 0:
        P0_DIR = Pn_DIR;
		P0_PU = Pn_PU;
		
        break;

    case 1:
        P1_DIR = Pn_DIR;
		P1_PU = Pn_PU;
		
        break;

    case 2:
        P2_DIR = Pn_DIR;
		P2_PU = Pn_PU;
		
        break;

    case 3:
        P3_DIR = Pn_DIR;
		P3_PU = Pn_PU;
		
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
* Function Name  : CH559P4Mode()
* Description    : CH559的P4端口初始化，P4默认是输入口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH559P4Mode( )
{
	//P4_DIR |= 0xff; 														   //置1设置为输出
	P4_DIR |= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
	//P4_PU |= 0xff;															   //启动p4口内部上拉
    P4_PU |= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
	P4_CFG |= bP4_DRV;														   //该位为0则P4口驱动能力5mA,为1时为20mA
}


/*******************************************************************************
* Function Name  : CH559GPIOInterruptInit()
* Description    : CH559GPIO中断初始化，其他引脚如P5.5\P1.4\P0.3\P5.7\P4.1\RXD0设置同理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH559GPIOInterruptInit()
{													   
	GPIO_IE &= ~bIE_IO_EDGE;												   //中断方式选择，该位为0则表示IO口电平中断，该位为1则表示IO口边沿中断
	GPIO_IE |= bIE_RXD1_LO; 												   //使能RXD1引脚的中断,其他引脚中断设置同理
}

