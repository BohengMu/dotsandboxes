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
#include <stdio.h>
#include <stdlib.h>


volatile enum EncoderState g_encoder_state = Neutral;

/*
 * set up encoder to be A input on pin 3.6 and B input on pin 3.5.
 * Pull down resistors are enabled to check A and B inputs for direction of rotation
 * Rising edge interrupts are enabled to check when encoder is turned
 */

void configure_encoder(void)
{
    Interrupt_enableInterrupt(INT_PORT3); //enable Port 3 interrupt on NVIC
    Interrupt_setPriority(INT_PORT3, 0); //set priority first
    P3->DIR &= ~(BIT6 | BIT5);   // set P3.6 as A input and P3.5 as B input
    P3->REN |= (BIT6 | BIT5); //enable pull up/pull down resistors on Port 3.6 and 3.5
    P3->OUT &= ~(BIT6 | BIT5); //select pull down resistor on Port 3.6 and Port 3.5
    P3->IE  |= BIT6;    // enable P3.6 interrupt
    P3->IES &= ~BIT6;   // rising edge interrupt on P3.6
}

/*
 * IRQ handler for port 3, which is triggered by the encoder
 * The handler will increment or decrement the variable encoder_count, to be used in the game logic
 * to determine which direction to draw a line.
 *
 */

void PORT3_IRQHandler(void)
{
    if(ENCODER1_A_INTERRUPT)//if P3.6 interrupted on A input
    {
        if(ENCODER1_B_HIGH) //if B input is high, increment count for clockwise
        {
            g_encoder_state =  CounterClockwise;
        }
        else //if B input is low, decrement count for counterclockwise
        {
            g_encoder_state = Clockwise;
        }
        P3->IFG &= ~BIT6; //reset interrupt flag for P3.6
    }

}

//end of file
