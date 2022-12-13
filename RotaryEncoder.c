/*
 * RotaryEncoder.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "RotaryEncoder.h"
#include "defines.h"
#include "msp.h"
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>

volatile enum EncoderState g_encoder_state = Neutral;
//extern vector g_input_vector;
extern volatile int g_current_input;
/*
 * set up encoder to be A input on pin 3.5 and B input on pin 3.6.
 * Pull Up resistors are set in PCB
 * Falling edge interrupts are enabled to check when encoder is turned
 */

void configure_encoder(void)
{
    Interrupt_enableInterrupt(INT_PORT3); //enable Port 3 interrupt on NVIC
    Interrupt_setPriority(INT_PORT3, 0); //set priority first
    P3->DIR &= ~(BIT6 | BIT5);   // set P3.5 as A input and P3.6 as B input
    P3->IE  |= (BIT5);    // enable P3.5 interrupt
    P3->IES |= (BIT5);   // falling edge interrupt on P3.5
}

/*
 * IRQ handler for encoder, which is triggered every 5 ms by InputInterrupt.c
 * The handler will set g_encoder_state based on direction of rotation, to be used by game logic
 * to determine which direction to draw a line.
 * Input queue only adds move if this is the first move to be processed
 * Input Queue: Set g_current_input to 6 if Counterclockwise, 5 is clockwise
 *
 */
void PORT3_IRQHandler(void)
{
    //if the encoder is turned, check if B input is already active high
    //B input high indicates clockwise direction, otherwise counterclockwise
    if(ENCODER1_A_INTERRUPT)
    {
        if(ENCODER1_B_HIGH)
        {
            g_encoder_state = Clockwise;
            if(g_current_input == 0)
            {
                g_current_input = 5;
            }
        }
        else
        {
            g_encoder_state = CounterClockwise;
            if(g_current_input == 0)
            {
                g_current_input = 6;
            }
        }
    }

    P3->IFG &= ~(BIT5); //reset interrupt flag for P3.5

}

//end of file
