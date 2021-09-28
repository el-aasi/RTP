/*
 * Driver_Led.h
 *
 * Created: 24/02/2021 00:42:39
 *  Author: mvdNo
 * 
 * Header file for the Led functionality.
 */ 


#include <stdint.h>
#ifndef DRIVER_LED_H_
#define DRIVER_LED_H_

void init_led();
	
void set_led(uint8_t led_no, uint8_t led_state);

void toggle_led(uint8_t led_no);

#endif /* DRIVER_LED_H_ */