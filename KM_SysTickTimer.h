
void KM_SysTickTimer_Init(void);
void KM_delay_ms(unsigned long );
void KM_delay_us(unsigned long );

#define STTIMER_STCTRL_R *((int*)0xE000E010)
#define STTIMER_STRELOAD_R *((int*)0xE000E014)
#define STTIMER_STCURRENT_R *((int*)0xE000E018)

