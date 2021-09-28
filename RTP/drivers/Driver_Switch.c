/*
 * Driver_Switch.c
 *
 * Created: 24/02/2021 00:42:16
 * 
 * Source file for the Button functionality
 */ 

#include <avr/io.h>
#include "Driver_Switch.h"
#include "Driver_Led.h"

/*
* Initialization of the hardware buttons.
* Sets the button port as input(logic 0).
* @return void
*/
void init_button(){
	// set PortC as input with internal pull-ups on
	DDRC = 0x00;
	PORTC = 0x3F;
	
	DDRD = 0x00;
	PORTD = 0x0C;
}

/*
* Function to get the state of the target button (on/off).
* @param uint8_t button_no, the target button
* @return uint8_t, returns true if the button is pressed.
*/
uint8_t get_buttonState(uint8_t button_no){
	
	if(button_no <= 5)
	{
		return (PINC & (1 << button_no)) >> button_no;
	}
	
	return ((PIND & (1 << (button_no - 4))) >> (button_no - 4));
}