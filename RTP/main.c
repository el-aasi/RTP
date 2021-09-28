/*
 * main.c
 *
 * Created: 29/08/2021 21:44:06
 * Author: LBS
 
 * System tick = 58 ticks pr. second
 */

 #include <stdio.h>
 #include <avr/io.h>
 #include <avr/sfr_defs.h>
 #include <stdint.h>

 // Drivers
 #include <rc_servo.h>
 #include <display_7seg.h>

 #include <ATMEGA_FreeRTOS.h>
 #include <semphr.h>

 #include <FreeRTOSTraceDriver.h>
 #include <stdio_driver.h>
 #include <serial.h>
 #include <Driver_Led.h>
 #include <Driver_Switch.h>

 // define two Tasks
 void task1( void *pvParameters );

 // define semaphore handle
 SemaphoreHandle_t xTestSemaphore;


 /*-----------------------------------------------------------*/
 void create_tasks_and_semaphores(void)
 {
	 // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	 // because it is sharing a resource, such as the Serial port.
	 // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	 if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	 {
		 xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		 if ( ( xTestSemaphore ) != NULL )
		 {
			 xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		 }
	 }

	 xTaskCreate(
	 task1
	 ,  "Task1"  // A name just for humans
	 ,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	 ,  NULL
	 ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	 ,  NULL );
 }

 /*-----------------------------------------------------------*/
 void task1( void *pvParameters )
 {
	 #if (configUSE_APPLICATION_TASK_TAG == 1)
	 // Set task no to be used for tracing with R2R-Network
	 vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	 #endif

	 TickType_t xLastWakeTime;
	 const TickType_t xFrequency = 29; // 100 ms

	 // Initialize the xLastWakeTime variable with the current time.
	 xLastWakeTime = xTaskGetTickCount();
	 
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		
		uint8_t buttonStates[8];
		uint8_t output = 0;
		uint8_t power = 1;
		uint8_t number_of_leds = 7;
		
		for(int8_t i = 0; i <= number_of_leds; i++)
		{
			uint8_t temp = get_buttonState(i);
			buttonStates[i] = temp;
			set_led(i, temp);
		}
		
		for (int8_t i = number_of_leds; i>=0; i--)
		{
			uint8_t buttonValue = 1 - buttonStates[number_of_leds - i];
			output +=  buttonValue * power;
			power *= 2;
		}
		
		display_7seg_display(output,0);
		
		if(output == 128)
		{
			rc_servo_setPosition(0, 100);
		}
		else if(output == 1)
		{
			rc_servo_setPosition(0, -100);
		}
		else
		{
			rc_servo_setPosition(0, 0);
		}

	}
 }

 /*-----------------------------------------------------------*/
 void initialiseSystem()
 {
	 init_led();
	 init_button();
	 
	DDRD = 0x00;
	PORTD = 0x0C;
	 
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	 
	// Create some tasks
	create_tasks_and_semaphores();
	 
	// Initialize drivers
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
	 
	rc_servo_initialise ();
 }

 /*-----------------------------------------------------------*/
 int main(void)
 {	
	 initialiseSystem(); // Must be done as the very first thing!!
	 printf("Program Started.\r\n");
	 vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	 /* Replace with your application code */
	 while (1)
	 {
	 }
 }
 