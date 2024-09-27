
#include "type.h"
#include "ch559.h"
#include "system.h"

volatile uint16_t SoftWatchdog = 0;

FunctionReference runBootloader = (FunctionReference)0xF400;
volatile bool OutputsEnabled = 0;

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
	SoftWatchdog = 0;
	
	while ( n ) 
	{
		mDelayuS( 1000 );
		-- n;
	}

	SoftWatchdog = 0;
}

#if defined(BOARD_MICRO)
// HIDman-micro doesn't have Serial port, so use dummy putchar function
int putchar(int c)
{
    return c;
}
#endif


void GPIOInit(void){
	
#if defined(BOARD_MICRO) // Pinouts for HIDman-micro
	//port1 setup
	P1_DIR = 0b11110000; // 0.4, 0.5, 0.6, 0.7 are keyboard/mouse outputs
	PORT_CFG = bP1_OC;	  // open collector
	P1_PU = 0x00;		  // no pullups
	P1 = 0b11110000;	  // default pin states

	//port2 setup
	P2_DIR = 0b00100000; // 2.5 is LED output
	PORT_CFG |= bP2_OC;	  // open collector
	P2_PU = 0x00;		  // no pullups
	P2 = 0b00100000;	  // LED off by default (i.e. high)

#elif defined(BOARD_PS2) // Pinouts for old PS2 version

	//port0 setup
	P0_DIR = 0b01110000; // 456 are red-green-blue LEDs
	PORT_CFG = bP0_OC;	 // Push-pull
	P0_PU = 0x00;		 // no pullups
	P0 = 0b01110000;	 // default pin states

	//port2 setup
	P2_DIR = 0b00000011; // 2.0/2.1 are PS2 outputs
	PORT_CFG |= bP2_OC;	 // open collector
	P2_PU = 0x00;		 // no pullups
	P3 = 0b00000011;	 // default pin states

	//port3 setup
	P3_DIR = 0b11111100; // 234567 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	 // open collector
	P3_PU = 0b00000001; // pullup on 1 for TXD
	P3 = 0b11111100;	 // default pin states

	//port4 setup
	P4_DIR = 0b00000000; // 4.6 is SWITCH
	P4_PU = 0b01000000;	 // pullup on switch
	P4_OUT = 0b00000000;


#else					  // Default pinouts (HIDman-AXP, HIDman-mini)
	//port0 setup
	P0_DIR = 0b11101010; // 0.3, 0.5, 0.6, 0.7 are all keyboard outputs, 0.4 is CTS (i.e. RTS on host), 0.1 is RTS (i.e. CTS on host)
	PORT_CFG |= bP0_OC;	 // open collector
	P0_PU = 0x00;		 // no pullups
	P0 = 0b11111010;	 // default pin states

	//port2 setup
	P2_DIR = 0b00110000; // 2.4, 2.5 are RED/GREEN LED outputs
	PORT_CFG |= bP2_OC;	 // open collector
	P2_PU = 0x00;		 // no pullups
	P2 = 0b00110000;	 // LEDs off by default (i.e. high)

	//port3 setup
	P3_DIR = 0b11100010; // 4 is switch, 5,6,7 are PS2 outputs, 1 is UART0 TXD
	PORT_CFG |= bP3_OC;	 // open collector
	P3_PU = 0b00010001; // pullup on 1 for TXD, 4 for switch
	P3 = 0b11110010;	 // default pin states

	//port4 setup
	P4_DIR = 0b00010100; //4.0 is RXD, 4.2 is Blue LED, 4.3 is MOUSE DATA (actually input, since we're faking open drain), 4.4 is TXD, 4.6 is SWITCH
	P4_PU = 0b01000000;	 // pullup on switch
	P4_OUT = 0b00000100; //LEDs off (i.e. HIGH), MOUSE DATA low (since it's switched by toggling input on and off, i.e. faking open drain)
#endif

}

void ClockInit(void){


	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;												 

#if defined(OSC_EXTERNAL)
	CLOCK_CFG |= bOSC_EN_XT;								  
	CLOCK_CFG &= ~bOSC_EN_INT;												
#endif

	CLOCK_CFG &= ~MASK_SYS_CK_DIV;

	CLOCK_CFG |= 6; 															  
	PLL_CFG = (24 << 0) | (6 << 5);
}
