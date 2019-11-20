#include "km_tm4c123gh6pm.h"
#include "KM_SysTickTimer.h"
#include "pll.h"
#include<stdio.h>


 extern int temp,hum;
void UART_Out_Char(char );
void RTC_LCD_DHT11(void);
	void Lcd_loop(void);

void UART2_Init(void)		//Init UART2
{
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2; //Enable Sysclk for UART2
	while(!(SYSCTL_PRUART_R & SYSCTL_RCGCUART_R2)); //UART1 sysclk not ready then wait
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; //Enable PortD clock
	while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3));//PORTD sysclk not ready then wait
	GPIO_PORTD_LOCK_R |=GPIO_LOCK_KEY;
	GPIO_PORTD_CR_R |=0X80;
	GPIO_PORTD_DEN_R |= 0xC0; //Set PD7 and PD6 Digital enable
	GPIO_PORTD_AFSEL_R |= 0xC0; //Set PD7 and PD6 to enable alternate function
	GPIO_PORTD_PCTL_R &= 0x00FFFFFF; //Clear PD7 and PD6 bits
	GPIO_PORTD_PCTL_R |= 0x11000000; //Set to UART function
	// *****************************************************************************
	//PORT B INTALIZATION
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //Enable PortB clock
	while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R1));//PORTB sysclk not ready then wait
	GPIO_PORTB_DEN_R |= 0x20; //Set PB5 Digital enable
		GPIO_PORTB_DIR_R |= 0x20;
	//GPIO_PORTB_AFSEL_R |= 0x20; //Set PB5 to enable alternate function
	GPIO_PORTB_PCTL_R &= 0xFF0FFFFF; //Clear PB5 bits
}


void UART2_Config(void)		//COnfig UART1
{
	UART2_CTL_R &= ~(UART_CTL_UARTEN); //Disable UART2 
	UART2_IBRD_R = 27; //Baud rate 9600
	UART2_FBRD_R = 9; //Baud rate 9600
	UART2_LCRH_R |= 0x60; //Word Length=8bits; Stop bit=1; Parity=disable; FIFO=disable 
//	UART1_CTL_R |= UART_CTL_LBE; //Enable loop back
	UART2_CTL_R |= UART_CTL_UARTEN; //Enable UART2
}
void UART2_OutString(char arr[])
{
	int i;
	for(i=0;arr[i];i++)
	{
			UART_Out_Char(arr[i]); 
	}
}
void UART_Out_Char(char temp)
{
	while((UART2_FR_R&UART_FR_TXFF) !=0); //If THR full wait
	UART2_DR_R = temp; //Load char into Data Register
}

char UART_In_Char(void)
{
	while((UART2_FR_R & UART_FR_RXFE) !=0); //If RHR empty wait
	return (UART2_DR_R & 0x000000FF); //Return raed char in Data Register
}


int main()
{
	char data,Fetch[100];
	KM_PLL_Init(); //System clock to 50Mhz
	KM_SysTickTimer_Init(); //Systik Init
	UART2_Init(); //UART1 Init
	UART2_Config(); //UART1 Config
	RTC_LCD_DHT11();
		 GPIO_PORTB_DATA_R &= ~0x20; // reset low
    KM_delay_ms(10);
    GPIO_PORTB_DATA_R |= 0x20;
	
	 UART2_OutString("AT+RST\r\n");
	KM_delay_ms(100);
			UART2_OutString("AT\r\n"); //Send char onto Tx line
	KM_delay_ms(100);
	UART2_OutString("AT+CWMODE=3\r\n") ;
	KM_delay_ms(100);
	UART2_OutString("AT+CWJAP=\"KernelMasters\",\"9963111084\"\r\n");
	KM_delay_ms(100);
	UART2_OutString("AT+CIFSR\r\n");
	KM_delay_ms(100);
	UART2_OutString("AT+CIPMUX=0\r\n");
	KM_delay_ms(100);
	UART2_OutString("AT+CWLAP\r\n");
	KM_delay_ms(100);
	UART2_OutString("AT+CIPMODE=0\r\n");
	KM_delay_ms(100);
	
	while(1)
{
	Lcd_loop();

//------------------------
UART2_OutString("AT+CIPSTATUS\r\n"); //Search String is  "OK".
	KM_delay_ms(100);
//--------------------------------
UART2_OutString("AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n"); //Search String is  "OK".
	KM_delay_ms(100);

//----------------------
sprintf(Fetch,"GET /page.php?temp=%d&hum=%d&dev=1\r\n\r\n", temp,hum);
UART2_OutString("AT+CIPSEND=50\r\n");
KM_delay_ms(500);
UART2_OutString(Fetch);
KM_delay_ms(100);
//--------------------------
UART2_OutString("AT+CIPCLOSE\r\n");
KM_delay_ms(100);	//Search String is  "OK".
	 
		data = UART_In_Char(); //Read char on Rx line
		data++;
	}
}