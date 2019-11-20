#include "km_tm4c123gh6pm.h"
#include "KM_SysTickTimer.h"
#include "pll.h"
#include "lcd.h"
#include "dht11.h"
#include "rtc.h"
 int temp,hum;
int data[7]={0};
int hum, hum_low, temp, temp_low, parity;

void RTC_LCD_DHT11()
{

	KM_PLL_Init(); //System clock to 50Mhz
	KM_SysTickTimer_Init(); //Systik Init
	GPIO_PORTE_Init(); //DHT11 Init
	DHT11_Init(); //Init DHT11
	I2C0_Init(); //I2C0 Init
	I2C0_Config(); //I2C0 Config	
	GPIO_PORTA_PORTD_Init();
	LCD_Init(); //LCD Init
}
void Lcd_loop(void){
		int a,b,i;
	if(RTC_Write()==0)	
	{
		a=RTC_Read(); //Receive data from Slave
		if(a==0)
		{
			LCD_cmd_4bit(0x01); //Clear LCD
			LCD_cmd_4bit(0x80); //First line beginning
			LCD_data_string("Time: ");
			b = LCD_BCDtoi(data[2]);
			LCD_itoa(b);
			LCD_data_string(":");
			b = LCD_BCDtoi(data[1]);
			LCD_itoa(b);
			LCD_data_string(":");
			b = LCD_BCDtoi(data[0]);
			LCD_itoa(b);
			KM_delay_ms(310);
		}
		
			DHT11_Master_Request(); //Master request
			if(DHT11_Slave_Response()) //Slave exists
			{
					hum= DHT11_Rx_8_Data();
					//hum = DHT11_Rx_8_Data();
					hum_low = DHT11_Rx_8_Data();
					temp= DHT11_Rx_8_Data();
				   //tem= DHT11_Rx_8_Data();
					temp_low = DHT11_Rx_8_Data();
					parity = DHT11_Rx_8_Data();
				
					if(DHT11_Verify_Parity(parity)) 	//Parity Correct??
					{
						LCD_cmd_4bit(0xC0); //First line beginning
						LCD_data_string("H: "); 
						LCD_itoa(hum); //Humidity Value
						LCD_cmd_4bit(0xC7); //Second line beginning
						LCD_data_string("T: "); 
						LCD_itoa(temp); //Temperature Value
					}
					else
					{
						LCD_cmd_4bit(0x01); //Clear LCD
						LCD_cmd_4bit(0xC0); //First line beginning
						LCD_data_string("Wrong Parity"); 
					}
			}
			else
			{
					LCD_cmd_4bit(0x01); //Clear LCD
					LCD_cmd_4bit(0xC0); //First line beginning
					LCD_data_string("Slave doesn't exists"); //Slave doesnt exists
			}		
			for(i=0;i<30;i++)
			KM_delay_ms(100);		
		//}
	}
	b++;
}

