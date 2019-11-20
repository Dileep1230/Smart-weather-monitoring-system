void GPIO_PORTE_Init(void);		//Port E init
void DHT11_Init(void); //DHT11 init
void DHT11_Master_Request(void);  //Master request
int DHT11_Slave_Response(void); //Slave Response
char DHT11_Rx_8_Data(void); //Receive Data
int DHT11_Verify_Parity(int ); //Verify Parity 

