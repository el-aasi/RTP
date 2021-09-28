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

SemaphoreHandle_t xSemaphore; 
void sampleTask(void *pvParameters);
void readTask(void *pvParameters);
int arr[100];

void create_tasks_and_semaphores(void)
{
	xSemaphore = xSemaphoreCreateCounting(1, 1);
	
	xTaskCreate(sampleTask, "Sample Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(readTask, "Read Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
}

void sampleTask(void *pvParameters)
{
	for(;;)
	{
		printf("Sample task attempting to take the semaphore\r\n");

		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		display_7seg_display(0,0);
		printf("Sample task started\r\n");
		
		for(int c = 0; c < 100; c++)
		{
			for(int i = 0; i < 1000; i++)
			{
				for(int i = 0; i < 100; i++)
				{
					;
				}
			}
			arr[c] = c;
		}
		
		xSemaphoreGive(xSemaphore);
		
		vTaskDelay(29);
	}
}

void readTask(void *pvParameters)
{
	for(;;)
	{
		printf("Read attempting to take the semaphore\r\n");
		
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		
		display_7seg_display(1,0);
		printf("Read task started\r\n");

		int sumHolder = 0;
		
		for(int c = 0; c < 100; c++)
		{
			sumHolder += arr[c];
			arr[c] = 0;
			
			for(int i = 0; i < 1000; i++)
			{
				for(int j = 0; j < 100; j++)
				{
					;
				}
			}
		}
	
		printf("%d\r\n", sumHolder/100);
		xSemaphoreGive(xSemaphore);	
		
		vTaskDelay(29);
	}
}

void initialiseSystem()
{
	stdio_initialise(ser_USART0);
	create_tasks_and_semaphores();
	
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
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
 