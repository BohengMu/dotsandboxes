/*
 * defines.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef DEFINES_H_
#define DEFINES_H_

//board size defines
#define ROWS 4
#define COLUMNS 5
#define LED_MATRIX_SIZE 32
#define LED_MATRIX_ROWS 26
#define LED_MATRIX_COLUMNS 32
#define GAME_STATE_ROWS (ROWS * 2 + 1)
#define GAME_STATE_COLUMNS (COLUMNS * 2 + 1)

//input defines
#define JOYSTICK_V ADC_MEM0
#define JOYSTICK_H ADC_MEM1
#define JOYSTICK_V_PORT P5
#define JOYSTICK_V_PIN  5
#define JOYSTICK_H_PORT P5
#define JOYSTICK_H_PIN 4
#define PUSH_INPUT P1->IN
#define PUSH_PIN 0x10
#define PLAYER1_PUSH_PIN 0x20
#define ADC14_CONVERSION_START ADC14->CTL0|=1
#define ADC14_CONVERSION_FINISHED ADC14->IFGR0

//encoder defines
#define ENCODER1_A_INTERRUPT P3->IFG & BIT5
#define ENCODER1_B_INTERRUPT P3->IFG & BIT6
#define ENCODER1_A_HIGH P3->IN & BIT5
#define ENCODER1_B_HIGH P3->IN & BIT6
#define ENCODER1_B_LOW !(P3->IN & BIT5)

//game state defines
#define MOVE_SELECTED 3
#define LINE_NOT_SELECTED -1
#define DOT_SELECTED 1
#define DOT_NOT_SELECTED 0

//color definition
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 3


//joystick state defines
#define JOYSTICK_NEUTRAL ADC14->MEM[0] > 11000 && ADC14->MEM[1] > 11000
#define JOYSTICK_UP ADC14->MEM[0] < 5000
#define JOYSTICK_DOWN ADC14->MEM[0] > 14000
#define JOYSTICK_LEFT ADC14->MEM[1] < 5000
#define JOYSTICK_RIGHT ADC14->MEM[1] > 13000

#define ADC14_CONVERSION_START_ADDR(REG)    (uint32_t)((REG<<ADC14_CTL1_CSTARTADD_OFS) & ADC14_CTL1_CSTARTADD_MASK)

#endif /* DEFINES_H_ */
//end of file
