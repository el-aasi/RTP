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
	PORTA = PORTA & 0b11111010;
	vTaskDelay(14);
	PORTA = PORTA & 0b00000101;
	
	for(;;)
	{
		vTaskDelay(14);
		
		printf("Sending\n\r");
		
			PORTA = PORTA & 0b11111010;
			vTaskDelay(14);
			PORTA = PORTA & 0b00000101;
		
	}
}

void initialiseSystem()
{
	DDRA = 0b00000101;
	PORTA = 0b00001010;
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
 