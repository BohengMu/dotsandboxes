/*
 * PushButton.h
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_
#include <stdbool.h>

void configure_push_button(void);

//state variables for pushbutton state
enum ButtonState{NotPressed, Pressed};

enum ButtonState check_button_state(void);


#endif /* PUSHBUTTON_H_ */
//end of file
