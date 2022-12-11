/*
 * defines.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef DEFINES_H_
#define DEFINES_H_


#define ROWS 4
#define COLUMNS 5
#define LED_MATRIX_SIZE 32
#define LED_MATRIX_ROWS 26
#define LED_MATRIX_COLUMNS 32
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
#define ENCODER1_A_INTERRUPT P3->IFG & BIT7
#define ENCODER1_B_INTERRUPT P3->IFG & BIT5

#define ENCODER1_A_HIGH P3->IN & BIT7
#define ENCODER1_B_HIGH P3->IN & BIT5
#define ENCODER1_B_LOW !(P3->IN & BIT5)

//#define ENCODER1_A_HIGH P3->IN & BIT7
//#define ENCODER1_B_HIGH P3->IN & BIT5
//#define ENCODER1_B_LOW !(P3->IN & BIT5)
#define JOYSTICK_NEUTRAL ADC14->MEM[0] > 11000 && ADC14->MEM[1] > 11000
#define JOYSTICK_UP ADC14->MEM[0] < 5000
#define JOYSTICK_DOWN ADC14->MEM[0] > 14000
#define JOYSTICK_LEFT ADC14->MEM[1] < 5000
#define JOYSTICK_RIGHT ADC14->MEM[1] > 13000

#define ADC14_CONVERSION_START_ADDR(REG)    (uint32_t)((REG<<ADC14_CTL1_CSTARTADD_OFS) & ADC14_CTL1_CSTARTADD_MASK)


//MAX6955 definitions
#define MAX6955_1_BaseAddress 0b11000000

#define reg_noOp 0x00
#define reg_decodeMode 0x01
#define reg_globalIntensity 0x02
#define reg_scanLimit 0x03
#define reg_configuration 0x04
#define reg_displayTest 0x07
#define reg_digitType 0x0C

#define reg_intensity10 0x10
#define reg_intensity32 0x11
#define reg_intensity54 0x12
#define reg_intensity76 0x13
#define reg_intensity10a 0x14
#define reg_intensity32a 0x15
#define reg_intensity54a 0x16
#define reg_intensity76a 0x17

#define reg_digit0 0x60
#define reg_digit1 0x61
#define reg_digit2 0x62
#define reg_digit3 0x63
#define reg_digit4 0x64
#define reg_digit5 0x65
#define reg_digit6 0x66
#define reg_digit7 0x67
#define reg_digit0a 0x68
#define reg_digit1a 0x69
#define reg_digit2a 0x6A
#define reg_digit3a 0x6B
#define reg_digit4a 0x6C
#define reg_digit5a 0x6D
#define reg_digit6a 0x6E
#define reg_digit7a 0x6F

#endif /* DEFINES_H_ */
//end of file
