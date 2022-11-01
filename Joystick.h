/*
 * Joystick.h
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_



//state variables for Joystick State
enum JoystickState{Zero, Up, Down, Left, Right, UpRight, UpLeft, DownRight, DownLeft};
void configure_ADC(void);
enum JoystickState check_ADC_state(void);

#endif /* JOYSTICK_H_ */
//end of file
