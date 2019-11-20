#include "km_tm4c123gh6pm.h"
#include "KM_SysTickTimer.h"

int num[10];

void GPIO_PORTA_PORTD_Init(void)		//PA and PD set to output
{
	SYSCTL_RCGCGPIO_R |= (0x9);	//Port A and PORT D Clock only Enable
	while(!((SYSCTL_PRGPIO_R)&(0x9)));	//Wait until GPIO Port F is enable
	GPIO_PORTA_PCTL_R &= (0x000FFFFF);		//Port A bit feilds to set GPIO function
	GPIO_PORTD_PCTL_R &= (0xFFFF0000);		//Port D bit feilds to set GPIO function
	GPIO_PORTA_DIR_R |= (0x000000E0);		//PA5 to PA7 to set output direction
	GPIO_PORTD_DIR_R |= (0x0000000F);		//PD0 to PD4 to set output direction 
	GPIO_PORTA_DEN_R |= (0x000000E0);	//PA5 to PA7 to digital enable
	GPIO_PORTD_DEN_R |= (0x0000000F);	//PD0 to PD3 to digital enable
}

void LCD_enable_pulse(void)
{
	GPIO_PORTA_DATA_R |=(0x40); //EN high
	KM_delay_ms(12); //12ms delay
	GPIO_PORTA_DATA_R &=~(0x40); //EN low
}

void LCD_write_higher_nibble(char data)
{
	GPIO_PORTD_DATA_R &= ~(0xFF); //Clear bits PD0 to PD7
	GPIO_PORTD_DATA_R = (data>>4); //Shifted Data into PD0 to PD3
	LCD_enable_pulse(); //EN low to high pluse
}

void LCD_write_lower_nibble(char data)
{
	GPIO_PORTD_DATA_R &= ~(0xFF); //Clear bits PD0 to PD7
	GPIO_PORTD_DATA_R = (data); //Load Data into PD0 to PD3	
	LCD_enable_pulse(); //EN low to high pluse
}
void LCD_cmd_8bit(char data)
{
	GPIO_PORTA_DATA_R &= ~(0xE0); //Clear RS(Command Reg), RW(Write mode) and EN bits
	GPIO_PORTD_DATA_R = (data); //Send Data to PORTD
	LCD_enable_pulse(); //EN low to high pluse
}


void LCD_cmd_4bit(char data)
{
	GPIO_PORTA_DATA_R &= ~(0xE0); //Clear RS, RW and EN bits
	LCD_write_higher_nibble(data); //write higher nibble into PD0-PD3
	LCD_write_lower_nibble(data); //Write lower nibble into PD0-PD3

}
void LCD_Init(void)
{
		GPIO_PORTA_PORTD_Init(); //Init PORTA and PORTD
		KM_delay_ms(15); // Wait for 15ms
		LCD_cmd_8bit(0x03);	
		KM_delay_ms(5); // Wait for 5ms
		LCD_cmd_8bit(0x03);	
		KM_delay_us(100); // Wait for 100us
		LCD_cmd_8bit(0x03);	
		LCD_cmd_8bit(0x02);
		LCD_cmd_4bit(0x28);  //LCD in 4 bit mode
		LCD_cmd_4bit(0x08); //LCD Display off
		LCD_cmd_4bit(0x01);	//Clear Screen Display
		LCD_cmd_4bit(0x07);	//Shift Display left 
				LCD_cmd_4bit(0x0E); //Display on cursor blinking
			LCD_cmd_4bit(0x06); //Increment cursor(shift cursor to right)
}

void LCD_data_char(char data)
{
	GPIO_PORTA_DATA_R &= ~(0xE0); //Clear RS, RW and EN bits
	GPIO_PORTA_DATA_R |= (0x80); //Set RS bit(Data Register)
	LCD_write_higher_nibble(data); //write higher nibble into PD0-PD3
	LCD_write_lower_nibble(data); //Write lower nibble into PD0-PD3
}

void LCD_data_string(char *str)
{
	int i;
	for(i=0;str[i];i++)
	{
			LCD_data_char(str[i]); 
	}
}

void LCD_int(int x) //Int to ASCII
{
	int r=0,y=0,z=0;
	while(x>0)
	{
			y=x%10;
			r=r*10+y;
			x=x/10;
	}
	while(r>0)
	{
		z=r%10;
		z+=48;
	LCD_data_char(z);
		r=r/10;
	}
	if(r==0)
		LCD_data_char('0');
}

unsigned int LCD_itoBCD(unsigned int x)
{
	unsigned int y;
	y=(((x/10)<<4)|(x%10));
	
	return y;
	
}

void LCD_itoa(unsigned int x)
{
	int p;
	int k=0;
	if(x!=0)
	{
	while(x>0)
	{
		num[k]=x%10;
		x=x/10;
		k++;
	}
	}
	else
	{
		num[k]=x;
		k++;
	}
	k--;
	for(p=k;p>=0;p--)
	{
		LCD_data_char(num[p]+48);
	}	
}

int LCD_BCDtoi(int BCD)
{
	int y;
	y=(((BCD>>4)*10)+(BCD & 0xF));
	return y;
}
