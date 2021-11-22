#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>

#include <Driver_Led.h>
#include <Driver_Switch.h>
#include <rc_servo.h>
#include <display_7seg.h>

void messenger(void *pvParameters);

void create_tasks_and_semaphores(void)
{
	xTaskCreate(messenger, "Sample Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
}

void messenger(void *pvParameters)
{		
	PORTA = PORTA & 0b00001111;
	int counter = 0;
	
	for(;;)
	{
		printf("Not sending\r\n");
		PORTA = PORTA | 0b00001111;
		
		vTaskDelay(50);
		
		printf("Sending\r\n");
		PORTA = PORTA & 0b11110000;
		
		vTaskDelay(50);
			
			counter++; 
	}
}

void initialiseSystem()
{
	DDRA =  0b00001111;
	PORTA = 0b11110000;
	stdio_initialise(ser_USART0);
	create_tasks_and_semaphores();
}

 int main(void)
{	
	initialiseSystem();
	printf("Program Started.\r\n");
	vTaskStartScheduler();
	
	while (1)
	{
		
	}
}
 