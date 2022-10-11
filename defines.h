/*
 * defines.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef DEFINES_H_
#define DEFINES_H_


#define ROWS 10
#define COLUMNS 10
#define JOYSTICK_V ADC_MEM0
#define JOYSTICK_H ADC_MEM1
#define JOYSTICK_V_PORT P5
#define JOYSTICK_V_PIN  5
#define JOYSTICK_H_PORT P5
#define JOYSTICK_H_PIN 4
#define PUSH_INPUT P1->IN
#define PUSH_PIN 0x06
#define ADC14_CONVERSION_START ADC14->CTL0|=1
#define ADC14_CONVERSION_FINISHED ADC14->IFGR0
#define ENCODER_A_INTERRUPT P3->IV & 0x0E
#define ENCODER_B_HIGH P3->IN & BIT5

#endif /* DEFINES_H_ */
//end of file
