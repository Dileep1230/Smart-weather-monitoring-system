
#include"KM_SysTickTimer.h"


void KM_SysTickTimer_Init(void)
{
	STTIMER_STCTRL_R &= ~(0x00000001); // STTIMER Disable
	STTIMER_STCURRENT_R = 30;
	STTIMER_STCTRL_R |= 0x00000004; // System clock enable
 	STTIMER_STCTRL_R &= ~(0x00000002); // Enable Polling (Disable Interrupt)
	STTIMER_STCTRL_R |=  0x00000001; // STTIMER Enable
}

// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
#define ms 50000
#define us 50

void KM_delay_ms(unsigned long delay){
  STTIMER_STRELOAD_R = (ms*delay)-1;  // number of counts to wait
  STTIMER_STCURRENT_R = 0;       // any value written to CURRENT clears
  while((STTIMER_STCTRL_R&0x00010000)==0){ // wait for count flag
  }
}
void KM_delay_us(unsigned long delay){
  STTIMER_STRELOAD_R = (us*delay)-1;  // number of counts to wait
  STTIMER_STCURRENT_R = 0;       // any value written to CURRENT clears
  while((STTIMER_STCTRL_R&0x00010000)==0){ // wait for count flag
  }
}
