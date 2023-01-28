#ifndef __PWM_H__
#define __PWM_H__

void InitPWM1(UINT8 polar);
void InitPWM2(UINT8 polar);
void InitPWM3(UINT8 polar);
#define SetPWMClk(CK_SE) (PWM_CK_SE = CK_SE)   //Frequency division, default clock Fsys
#define SetPWMCycle(Cycle) (PWM_CYCLE = Cycle) //Set the cycle period
#define SetPWM1Dat(dat) (PWM_DATA = dat)	   //Set PWM output duty cycle
#define SetPWM2Dat(dat) (PWM_DATA2 = dat)
#define PWMPINAlter()              \
	{                              \
		P4_DIR |= bPWM2_ | bPWM1_; \
		PIN_FUNC |= bPWM1_PIN_X;   \
	} //Set PWM pin mapping


#endif