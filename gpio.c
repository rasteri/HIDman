
#include "type.h"
#include "ch559.h"
#include "system.h"
#include "gpio.h"

/*******************************************************************************
* Function Name  : CH559GPIODrivCap(UINT8 Port,UINT8 Cap)
* Description    : �˿�0��1��2��3������������
* Input          : UINT8 Port�˿�ѡ��(0��1��2��3)
                   UINT8 Cap��������ѡ��((0)5mA��(1)20mA(ע��:P1����10mA))
* Output         : None
* Return         : SUCCESS�ɹ�
                   FAILʧ��
*******************************************************************************/
UINT8 CH559GPIODrivCap(UINT8 Port,UINT8 Cap)
{
    if (Port >= 4)
    {
    	return FAIL;
    }
    
    if (Cap == 0)
    {                                                               //�����������5mA
        PORT_CFG &= ~(bP0_DRV << Port);
    }
    else
    {		
        PORT_CFG |= (bP0_DRV << Port);                                             //�����������20mA
    }

    return SUCCESS;
}

/*******************************************************************************
* Function Name  : CH559GPIOModeSelt(UINT8 Port,UINT8 Mode,UINT8 PinNum)
* Description    : �˿�0��1��2��3����ģʽ����
* Input          : UINT8 Port�˿�ѡ��(0��1��2��3)
                   UINT8 Cap������ʽѡ��(bPn_OC & Pn_DIR & Pn_PU)
                   0(000)�������룬��������
                   1(001)�������룬��������
                   2(01x)������������ߵ͵�ƽǿ������
                   3(100)����©�������������֧�����룻
                   4(110)����©�����������,��ת������ɵ͵���ʱ��������2��ʱ�ӵĸߵ�ƽ
                   5(101)��׼˫��(��׼51ģʽ)����©�����������
                   6(111)��׼˫��(��׼51ģʽ)����©���������������ת������ɵ͵���ʱ��������2��ʱ�ӵĸߵ�ƽ
                   UINT8 PinNum(����ѡ��0-7)
* Output         : None
* Return         : SUCCESS�ɹ�
                   FAILʧ��
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
    case 0:                              //�����룬������
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 1:                              //�����룬������
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU |= 1<<PinNum;
    	
    	break;
    	
    case 2:                              //����������ߵ͵�ƽǿ����
    	PORT_CFG &= ~(bP0_OC << Port);
    	Pn_DIR |= (1<<PinNum);
    	
    	break;
    	
    case 3:                              //��©�������������֧������
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 4:                              //��©�����������,��ת������ɵ͵���ʱ��������2��ʱ�ӵĸߵ�ƽ
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR |= 1<<PinNum;
    	Pn_PU &= ~(1<<PinNum);
    	
    	break;
    	
    case 5:                              //��׼˫��(��׼51ģʽ)����©�����������
    	PORT_CFG |= (bP0_OC << Port);
    	Pn_DIR &= ~(1<<PinNum);
    	Pn_PU |= 1<<PinNum;
    	
    	break;
    	
    case 6:                              //׼˫��(��׼51ģʽ)����©���������������ת������ɵ͵���ʱ��������2��ʱ�ӵĸߵ�ƽ
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
* Description    : CH559��P4�˿ڳ�ʼ����P4Ĭ���������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH559P4Mode( )
{
	//P4_DIR |= 0xff; 														   //��1����Ϊ���
	P4_DIR |= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
	//P4_PU |= 0xff;															   //����p4���ڲ�����
    P4_PU |= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
	P4_CFG |= bP4_DRV;														   //��λΪ0��P4����������5mA,Ϊ1ʱΪ20mA
}


/*******************************************************************************
* Function Name  : CH559GPIOInterruptInit()
* Description    : CH559GPIO�жϳ�ʼ��������������P5.5\P1.4\P0.3\P5.7\P4.1\RXD0����ͬ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH559GPIOInterruptInit()
{													   
	GPIO_IE &= ~bIE_IO_EDGE;												   //�жϷ�ʽѡ�񣬸�λΪ0���ʾIO�ڵ�ƽ�жϣ���λΪ1���ʾIO�ڱ����ж�
	GPIO_IE |= bIE_RXD1_LO; 												   //ʹ��RXD1���ŵ��ж�,���������ж�����ͬ��
}

