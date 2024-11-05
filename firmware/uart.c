#include "type.h"
#include "ch559.h" 
#include "system.h"
#include "uart.h"

/*******************************************************************************
* Function Name  : CH559UART0Alter()
* Description    : CH559����0����ӳ��,����ӳ�䵽P0.2��P0.3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH559UART0Alter(void)
{
    PORT_CFG |= bP0_OC;
    P0_DIR |= bTXD_;
    P0_PU |= bTXD_ | bRXD_;
    PIN_FUNC |= bUART0_PIN_X;                                                  //����ӳ�䵽P0.2��P0.3
}

/*******************************************************************************
* Function Name  : InitUART0()
* Description    : CH559����0��ʼ��,Ĭ��ʹ��T1��UART0�Ĳ����ʷ�����,Ҳ����ʹ��T2
                   ��Ϊ�����ʷ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitUART0(void)
{
    UINT32 x;
    UINT8 x2; 

#ifdef DEBUG
    //CH559UART0Alter();
#endif
    
    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //����0ʹ��ģʽ1
                                                                               //ʹ��Timer1��Ϊ�����ʷ�����
    RCLK = 0;                                                                  //UART0����ʱ��
    TCLK = 0;                                                                  //UART0����ʱ��
    PCON |= SMOD;
    x = 10 * FREQ_SYS / BUAD_RATE / 16;                                             //���������Ƶ��ע��x��ֵ��Ҫ���                            
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x++;                                                       //��������

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20��Timer1��Ϊ8λ�Զ����ض�ʱ��
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1ʱ��ѡ��
    TH1 = 0-x;                                                                 //12MHz����,buad/12Ϊʵ�������ò�����
    TR1 = 1;                                                                   //������ʱ��1

	REN = 1;                                                                   //����0����ʹ��
	
	TI = 1;


}

/*******************************************************************************
* Function Name  : CH559UART0RcvByte()
* Description    : CH559UART0����һ���ֽ�
* Input          : None
* Output         : None
* Return         : SBUF
*******************************************************************************/
UINT8  CH559UART0RcvByte(void)
{
    while(RI == 0);                                                            //��ѯ���գ��жϷ�ʽ�ɲ���
    RI = 0;
    return SBUF;
}

static BOOL volatile s_sent = FALSE;

void SetUart0Sent(void)
{
	s_sent = TRUE;
}


/*******************************************************************************
* Function Name  : CH559UART0SendByte(UINT8 SendDat)
* Description    : CH559UART0����һ���ֽ�
* Input          : UINT8 SendDat��Ҫ���͵�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH559UART0SendByte(UINT8 SendDat)
{
	s_sent = FALSE;
	SBUF = SendDat;
	while (!s_sent);
}

void CH559UART0SendData(UINT8 *pData, UINT8 len)
{
	while (len-- > 0)
	{
		s_sent = FALSE;
		SBUF = *pData++;
		while (!s_sent);
	}
}
