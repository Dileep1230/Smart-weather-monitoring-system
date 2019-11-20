#include "km_tm4c123gh6pm.h"
#include "KM_SysTickTimer.h"

extern int hum, hum_low, temp, temp_low, parity;

void GPIO_PORTE_Init(void)		//Port E init
{
	SYSCTL_RCGCGPIO_R |= (0x10);	//Port A and PORT D Clock only Enable
	while(!((SYSCTL_PRGPIO_R)&(0x10)));	//Wait until GPIO Port E is enable
	GPIO_PORTE_PCTL_R &=(0xFFFFFF0F);		//PE1 bit feilds to set GPIO function
	GPIO_PORTE_DEN_R |= (0x02);	//PE1 digital enable
}

void DHT11_Init(void) //DHT11 init
{
	int i;
	for(i=0;i<10;i++)			//2sec Delay
	{
			KM_delay_ms(200);
	}
}

void DHT11_Master_Request(void)  //Master request
{
	GPIO_PORTE_DIR_R |= (0x02);		//PE1 to set output direction
	GPIO_PORTE_DATA_R &= ~(0x02); //Clear PE1 bit as Low
	KM_delay_ms(20);
	GPIO_PORTE_DATA_R |= (0x02); //Set PE1 bit as High
}

int DHT11_Slave_Response(void)
{
	volatile int counter=0; 
	GPIO_PORTE_DIR_R &= ~(0x02);		//PE1 to set input direction
	while((GPIO_PORTE_DATA_R&0x02)!=0){
		counter++;
		if (counter == 200)
		return 0; // device offline
	}
	while((GPIO_PORTE_DATA_R&0x02)==0){;}
	while((GPIO_PORTE_DATA_R&0x02)!=0){;} //While PE1 is high, wait
	return 1;		//Slave exists
}
	
char DHT11_Rx_8_Data(void)
{
	char data=0;
	int i;
	for(i=0;i<8;i++)
	{
		while((GPIO_PORTE_DATA_R & 0x02) == 0){;} //While PE1 is low, wait
			KM_delay_us(30);
			if((GPIO_PORTE_DATA_R & 0x02) == 0) //PE1 is low
				data <<= 1; //Bit 0
			else
			{
				data <<= 1; 	//Bit 1
				data |= 0x1;
			while((GPIO_PORTE_DATA_R & 0x02) !=0);
			}
	}
	return data;
}

int DHT11_Verify_Parity(int parity)
{
	int result;
	result = hum + hum_low + temp + temp_low;  //checksum
	if(result == parity) //Checksum == parity ?
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


