
#include "Type.h"
#include "CH559.h"
#include "System.h"

/*******************************************************************************
* Function Name  : CfgFsys( )
* Description    : CH559时钟选择和配置函数,默认使用内部晶振12MHz，如果定义了FREQ_SYS可以
                   根据PLL_CFG和CLOCK_CFG配置得到，公式如下：
                   Fsys = (Fosc * ( PLL_CFG & MASK_PLL_MULT ))/(CLOCK_CFG & MASK_SYS_CK_DIV);
                   具体时钟需要自己配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void CfgFsys()	
{
	SAFE_MOD = 0x55;														   //开启安全模式
	SAFE_MOD = 0xAA;												 
//	CLOCK_CFG |= bOSC_EN_XT;												   //使能外部晶振										  
//	CLOCK_CFG &= ~bOSC_EN_INT;												
//	CLOCK_CFG &= ~MASK_SYS_CK_DIV;
//	CLOCK_CFG |= 6; 														   //配置系统时钟48MHz
//	CLOCK_CFG |= 8; 														   //配置系统时钟36MHz
//	CLOCK_CFG |= 10;														   //配置系统时钟28.8MHz
//	CLOCK_CFG |= 12;														   //配置系统时钟24MHz
//	CLOCK_CFG |= 16;														   //配置系统时钟18MHz	

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;

#if 1
	//配置系统时钟48MHz
	CLOCK_CFG |= 6; 															  
	PLL_CFG = (24 << 0) | (6 << 5);
#else
	//配置系统时钟56MHz
	CLOCK_CFG |= 6; 															  
	PLL_CFG = (28 << 0) | (7 << 5);
#endif	
	
	SAFE_MOD = 0xFF;														   //关闭安全模式  
//	如果修改主频，要同时修改FREQ_SYS，否则会造成延时函数不准
}


/*******************************************************************************
* Function Name  : mDelayus(UNIT16 n)
* Description    : us延时函数
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/ 
void mDelayuS( UINT16 n )  // 以uS为单位延时
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
* Description    : ms延时函数
* Input          : UNIT16 n
* Output         : None
* Return         : None
*******************************************************************************/
void	mDelaymS( UINT16 n )                                                  // 以mS为单位延时
{
	while ( n ) 
	{
		mDelayuS( 1000 );
		-- n;
	}
}

