/*
 * Driver_Switch.h
 *
 * Created: 24/02/2021 00:42:56
 *  Author: mvdNo
 * 
 * Header file for the Button functionality.
 */ 


#ifndef DRIVER_SWITCH_H_
#define DRIVER_SWITCH_H_

 /*
  * Conditional group macro (TEST).
  * Used for unit testing using the google framework.
  */
#define TEST

 /*
 * Conditional group block (TEST).
 * Will only be compiled when TEST is defined before this block in the code.
 * Used for unit testing using the google framework.
 */
#ifdef TEST
void test_button_init();
void test_main();
#endif

void init_button();

uint8_t get_buttonState(uint8_t button_no);

#endif /* DRIVER_SWITCH_H_ */