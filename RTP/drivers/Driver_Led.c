/*
 * Driver_Led.c
 *
 * Created: 24/02/2021 00:42:02
 *  Author: mvdNo
 * 
 * Source file for the Led functionality
 */ 

#include "Driver_Led.h"
#define DDR_LED DDRA
#define PORT_LED PORTA

//#include <xc.h>
#include <avr/io.h>


/*
* Initialisation of the hardware led's.
* Sets the led port as output(logic 1), and turns them off at start (active low).
* @return void
*/
void init_led(){
	DDR_LED |= 0xFF;	
	PORT_LED |= 0xFF;	
}

/*
* Function to set a specifick led to a specifick state.
* @param uint8_t led_no, The target led
* @param uint8_t led_state, the state to put the target led at
* @return void
*/
void set_led(uint8_t led_no, uint8_t led_state){
	if(led_state == 1){
		PORT_LED |= _BV(led_no);
	}else{
		PORT_LED &= ~_BV(led_no);
		}
}

/*
* Function to toggle the state of a specifick led.
* @param uint8_t led_no, The target led
* @return void
*/
void toggle_led(uint8_t led_no){
	PORT_LED ^= _BV(led_no);
}