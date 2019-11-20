#include "km_tm4c123gh6pm.h"

#define slave (0x68<<1)

void I2C0_Init(void)		//Init I2C0
{
	SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0; //Enable Sysclk for SSI2
	while(!(SYSCTL_PRI2C_R & SYSCTL_PRI2C_R0)); //UART1 sysclk not ready then wait
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //Enable PortB clock
	while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1));//PORTB sysclk not ready then wait
	GPIO_PORTB_AFSEL_R |= 0x0C; //Set PB2 to PB3 to enable alternate function
	GPIO_PORTB_ODR_R |= 0x08; //Pull up for PB3 (SDA pin) 
	GPIO_PORTB_DEN_R |= 0x0C; //Set PB4 to PB7 Digital enable
	GPIO_PORTB_PCTL_R &= 0xFFFF00FF; //Clear PB4 to PB7 bits
	GPIO_PORTB_PCTL_R |= 0x00003300; //Set to SSI function
}

void I2C0_Config(void)		//Config I2C0
{
	I2C0_MCR_R |= 0x10; //Master Mode for I2C0
	I2C0_MTPR_R =24; //50Mhz, 100kbps
}

int RTC_Write(void)  //Master Write/Transmit and Slave Receive
{
	int i;
	int date[]={00,59,11,2,26,8,19};

	I2C0_MSA_R = slave; //Slave address 68H and Master transmit
	I2C0_MSA_R &= ~(0x01); 
	I2C0_MDR_R &= ~(0xFF); //Clearing MDR data
	I2C0_MDR_R =0x00; //00H address of RTC to Data Reg
	I2C0_MCS_R = 0x03; //Set for Start and run bit
	
	for(i=0;i<7;i++) //RTC Write
	{
		while(((I2C0_MCS_R&0x01)!=0)){;}	//Wait until busy bit is 1
		while((I2C0_MCS_R&0x02)!=0) {;}//Error bit=0
		I2C0_MDR_R &= ~(0xFF); //Clearing MDR data
		I2C0_MDR_R = date[i];
		I2C0_MCS_R = 0x01; //Set for busy bit
	}
	
	I2C0_MCS_R = 0x05; //Set
	while(((I2C0_MCS_R&0x01)!=0)){;}	//Wait until busy bit is 1
		return (I2C0_MCS_R&0x0E);//Error bit=0 
}
	
extern int data[7];
int RTC_Read(void)
{
	int i;
	
	I2C0_MSA_R = slave; //Slave address 68H and Master transmit
	I2C0_MSA_R &= ~(0x1); //Write
	I2C0_MDR_R &= ~(0xFF); //Clearing MDR data
	I2C0_MDR_R =0x00;
	I2C0_MCS_R = 0x03; //Set Data Ack, start , run
	
	while(((I2C0_MCS_R&0x01)!=0)){;}
	I2C0_MSA_R = slave; 
	I2C0_MSA_R |= 0x01; //Read
	I2C0_MCS_R = 0x0B; //Set for Data Ack and run bit
	
	for(i=0;i<7;i++) //RTC Read
	{
		while(((I2C0_MCS_R&0x01)!=0)){;}	//Wait until busy bit is 1
		while(((I2C0_MCS_R&0x02)!=0)){;}
	
		I2C0_MDR_R &= ~(0xFF); //Clearing MDR data
		data[i] = I2C0_MDR_R; //Read Data from MDR
		I2C0_MCS_R = 0x09; //Set for busy bit
	}	
			
	I2C0_MCS_R = 0x05; //Set stop and run
	while(((I2C0_MCS_R&0x01)!=0)){;}	//Wait until busy bit is 1

	return (I2C0_MCS_R&0x0E); //Error service 
	
}

