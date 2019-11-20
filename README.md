# Smart-Weathering-Monitoring-System
In this Project I used a customised TM4C123GH6PM board,for which it has a on board DHT11(temperature and humidity sensor), a Monochrome 16*2 LCD, RTC(DS1307),wifi module ESP8266.

Intialization steps:
--------------------
I have Initailized LCD and RTC modules using the predefined steps in manual and we checked the working of wifi module using the basic AT commands.

Working:
-------
In my project interfaced DHT11 using single wire bi-directional protocal,RTC using I2C,ESP8266 using UART protocols.

DHT11 generates the temperature and humidty data for every two seconds so i have collected the data and displayed it on to a LCD along with the time stamp data collected from RTC. The same data collected from the devices is being uploaded into the cloud simultaneously with the help of wifi module.
