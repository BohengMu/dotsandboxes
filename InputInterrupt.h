/*
 * InputInterrupt.h
 *
 *  Created on: Nov 1, 2022
 *      Author: James Tsai
 */

#ifndef INPUTINTERRUPT_H_
#define INPUTINTERRUPT_H_

void configure_systick(void);
/* global int variable that indicates current input
 * 0 = no input
 * 1 = Joystick Up
 * 2 = Joystick Down
 * 3 = Joystick Left
 * 4 = Joystick Right
 * 5 = Button Pressed
 * 6 = Encoder Clockwise
 * 7 = Encoder CounterClockwise
 *
*/
volatile int g_current_input;

#endif /* INPUTINTERRUPT_H_ */
