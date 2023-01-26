
#include "type.h"
#include "ch559.h"
#include "timer2.h"

/*******************************************************************************
* Function Name  : mTimer2Setup(UINT8 T2Out)
* Description    : CH559��ʱ2��ʼ��
* Input          : UINT8 T2Out,�Ƿ�����T2���ʱ��
                   0�����������
                   1���������  
* Output         : None
* Return         : None
*******************************************************************************/
void mTimer2Setup(UINT8 T2Out)
{
    RCLK = 0;
    TCLK = 0;
    CP_RL2 = 0;                                                                //�����Զ����ض�ʱ������
    if(T2Out)
    {
	      T2MOD |= T2OE;                                                        //�Ƿ�����T2���ʱ��,�������ʱ��=1/2��ʱ��2�����
    }
    else
    {
	      T2MOD &= ~T2OE;
    }
}

/*******************************************************************************
* Function Name  : mTimer2Init(UINT16 Tim)
* Description    : CH559 T2��ʱ������ֵ                   
* Input          : UINT16 Tim,��ʱ����ֵ
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
* Description    : CH559��ʱ������2 T2EX���Ų�׽���ܳ�ʼ��
                   UINT8 mode,���ز�׽ģʽѡ��
                   0:T2ex���½��ص���һ���½���
                   1:T2ex�������֮��
                   3:T2ex�������ص���һ��������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void T2exCaptureSetup(UINT8 mode)
{
    C_T2  = 0;
    EXEN2 = 1; 
    CP_RL2 = 1;                                                                //����T2ex�Ĳ�׽����
    T2MOD |= mode << 2;                                                        //���ز�׽ģʽѡ��
}

/*******************************************************************************
* Function Name  : T2CaptureSetup(UINT8 mode)
* Description    : CH559��ʱ������2 T2���Ų�׽���ܳ�ʼ��T2
                   UINT8 mode,���ز�׽ģʽѡ��
                   0:T2ex���½��ص���һ���½���
                   1:T2ex�������֮��
                   3:T2ex�������ص���һ��������
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
    T2MOD &= ~T2OE;                                                            //ʹ��T2���Ų�׽����
    T2MOD |= (mode << 2) | bT2_CAP1_EN;                                        //���ز�׽ģʽѡ��
}

