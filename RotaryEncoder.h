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

//state variables for Encoder State;
enum EncoderState{Neutral, Clockwise, CounterClockwise};

enum EncoderState check_encoder_state(void);
#endif /* ROTARYENCODER_H_ */
