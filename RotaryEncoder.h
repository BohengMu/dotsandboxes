/*
 * RotaryEncoder.h
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "msp.h"
#include "driverlib.h"
#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

volatile int16_t encoderCount;
void configureEncoder(void);



#endif /* ROTARYENCODER_H_ */
