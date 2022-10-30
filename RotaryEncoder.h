/*
 * RotaryEncoder.h
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */


#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_
#include <stdint.h>

extern volatile int16_t g_encoder_count;
void configure_encoder(void);


#endif /* ROTARYENCODER_H_ */
