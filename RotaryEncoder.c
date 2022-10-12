/*
 * RotaryEncoder.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "RotaryEncoder.h"
#include "Defines.h"
#include "msp.h"
#include "driverlib.h"


volatile int16_t encoderCount;

/*
 * set up encoder to be on pin 3.6 and 3.5
 */
void configureEncoder(void)
{
    NVIC->ISER[1] = 1 << ((PORT3_IRQn) & 31); //enable P3 interrupt

    //Encoder on P3.6 and P3.5
    P3->DIR &= ~BIT6;   // A input
    P3->IE  |= BIT6;    // enable P3.6 interrupt
    P3->IES &= ~BIT6;   // rising edge
    P3->DIR &= ~BIT5;   // B input
}

/*
 * IRQ hander for port 3, which is triggered by the encoder
 */
void PORT3_IRQHandler(void)
{
    if(ENCODER_A_INTERRUPT){ //if P3 interrupted on A input
        if(ENCODER_B_HIGH){ //if B input is high, decrement count
            encoderCount--;
        }
        else{
            encoderCount++; //if A input is high, decrement count
        }
    }
}

