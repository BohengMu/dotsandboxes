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
extern volatile bool g_button_pressed;

#endif /* PUSHBUTTON_H_ */
