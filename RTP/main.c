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

SemaphoreHandle_t xInterruptSemaphore; 
int currentState = 0;
int buffer[100];
int index = 0; 

void fakeInterruptFunction(void *pvParameters);
void receiveMessage(void *pvParameters);

void create_tasks_and_semaphores(void)
{
	xInterruptSemaphore = xSemaphoreCreateCounting(1, 0);
	xTaskCreate(fakeInterruptFunction, "Fake Interrupt", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 99, NULL);
	xTaskCreate(receiveMessage, "Sample Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
}

void fakeInterruptFunction(void *pvParameters)
{	
	for(;;)
	{
		if(currentState == 0)
		{
			int interruptPin = 0;
			interruptPin = PINA & 0b00010000;
					
			if(interruptPin == 0)
			{
				currentState = 1;
				printf("Starting communication\n");
				xSemaphoreGive(xInterruptSemaphore);
			}
		}
		
		vTaskDelay(1);
	}
}

void receiveMessage(void *pvParameters)
{
	for(;;)
	{	
		if(currentState == 0)
		{
			xSemaphoreTake(xInterruptSemaphore, portMAX_DELAY);
			vTaskDelay(6);
		}
		
		int stopPin, dataPin = 0;
		stopPin = PINA & 0b10000000;
		dataPin = PINA & 0b01000000;
		
		if(stopPin == 0)
		{
			currentState = 0;
			
			printf("Message: ");
			for(int i = 0; i <= index - 2; i++)
			{
				printf("%d", buffer[i]);
			}
			printf("\n");
			
			for(int i = 0; i <= index; i++)
			{
				buffer[i] = -1;
			}
			
			
			index = 0;
			continue;
		}
		
		if(dataPin == 0)
		{
			buffer[index] = 1;
		}
		else
		{
			buffer[index] = 0;
		}
		
		index++;
		
		vTaskDelay(6);
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
	printf("Program Started.\n");
	vTaskStartScheduler();
	
	while (1)
	{
		
	}
}
 