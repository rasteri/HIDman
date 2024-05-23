
#include "type.h"
#include "ch559.h"
#include "system.h"

/*******************************************************************************
* Function Name  : CfgFsys( )
* Description    : CH559ʱ��ѡ������ú���,Ĭ��ʹ���ڲ�����12MHz�����������FREQ_SYS����
                   ����PLL_CFG��CLOCK_CFG���õõ�����ʽ���£�
                   Fsys = (Fosc * ( PLL_CFG & MASK_PLL_MULT ))/(CLOCK_CFG & MASK_SYS_CK_DIV);
                   ����ʱ����Ҫ�Լ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void CfgFsys()	
{
	SAFE_MOD = 0x55;														   //������ȫģʽ
	SAFE_MOD = 0xAA;												 

#if defined(OSC_EXTERNAL)
	CLOCK_CFG |= bOSC_EN_XT;												   //ʹ���ⲿ����										  
	CLOCK_CFG &= ~bOSC_EN_INT;												
#endif

//	CLOCK_CFG &= ~MASK_SYS_CK_DIV;
//	CLOCK_CFG |= 6; 														   //����ϵͳʱ��48MHz
//	CLOCK_CFG |= 8; 														   //����ϵͳʱ��36MHz
//	CLOCK_CFG |= 10;														   //����ϵͳʱ��28.8MHz
//	CLOCK_CFG |= 12;														   //����ϵͳʱ��24MHz
//	CLOCK_CFG |= 16;														   //����ϵͳʱ��18MHz	

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;

	//GLOBAL_CFG |= bWDOG_EN;

#if 1
	//����ϵͳʱ��48MHz
	CLOCK_CFG |= 6; 															  
	PLL_CFG = (24 << 0) | (6 << 5);
#else
	//����ϵͳʱ��56MHz
	CLOCK_CFG |= 6; 															  
	PLL_CFG = (28 << 0) | (7 << 5);
#endif	
	
	SAFE_MOD = 0xFF;														   //�رհ�ȫģʽ  
//	����޸���Ƶ��Ҫͬʱ�޸�FREQ_SYS������������ʱ������׼
}


/*******************************************************************************
* Function Name  : mDelayus(UNIT16 n)
* Description    : us��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void mDelayuS( UINT16 n )  // ��uSΪ��λ��ʱ
{
	while ( n ) {  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
		++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef	FREQ_SYS
#if		FREQ_SYS >= 14000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 16000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 18000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 20000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 22000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 24000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 26000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 28000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 30000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 32000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 34000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 36000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 38000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 40000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 42000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 44000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 46000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 48000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 50000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 52000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 54000000
		++ SAFE_MOD;
#endif
#if		FREQ_SYS >= 56000000
		++ SAFE_MOD;
#endif
#endif
		-- n;
	}
}


/*******************************************************************************
* Function Name  : mDelayms(UNIT16 n)
* Description    : ms��ʱ����
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	mDelaymS( UINT16 n )                                                  // ��mSΪ��λ��ʱ
{
	// reset watchdog, as this function is only used by USB routines that sometimes take a while to return
	WDOG_COUNT = 0x00;
	
	while ( n ) 
	{
		mDelayuS( 1000 );
		-- n;
	}

	WDOG_COUNT = 0x00;
}

