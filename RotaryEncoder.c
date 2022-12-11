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
 * set up encoder to be A input on pin 3.5 and B input on pin 3.7.
 * Pull down resistors are enabled to check A and B inputs for direction of rotation
 * Rising edge interrupts are enabled to check when encoder is turned
 */

void configure_encoder(void)
{
    Interrupt_enableInterrupt(INT_PORT3); //enable Port 3 interrupt on NVIC
    Interrupt_setPriority(INT_PORT3, 0); //set priority first
    P3->DIR &= ~(BIT7 | BIT5);   // set P3.7 as A input and P3.5 as B input
   // P3->REN |= (BIT7 | BIT5); //enable pull up/pull down resistors on Port 3.7 and 3.5
    //P3->OUT |= (BIT7 | BIT5); //select pull up resistor on Port 3.7 and Port 3.5
    P3->IE  |= (BIT5);    // enable P3.7 and P3.5 interrupt
    P3->IES &= ~(BIT5);   // falling edge interrupt on P3.7 and P3.5
}

/*
 * IRQ handler for encoder, which is triggered every 5 ms by InputInterrupt.c
 * The handler will set g_encoder_state based on direction of rotation, to be used by game logic
 * to determine which direction to draw a line.
 * Inpue Queue: Set g_current_input to 6 if Counterclockwise, 5 is clockwise
 *
 */
void PORT3_IRQHandler(void)
{
    if(ENCODER1_B_INTERRUPT)
    {
        if(ENCODER1_A_HIGH)
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
//    if(ENCODER1_B_INTERRUPT)// if B interrupt, check pin values
//    {
//        if(GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == 0)
//        {
//            g_encoder_state = CounterClockwise;
//            if(g_current_input == 0)
//            {
//                g_current_input = 6;
//            }
//        }
//        else
//        {
//            g_encoder_state = Clockwise;
//            if(g_current_input == 0)
//            {
//                g_current_input = 5;
//            }
//        }
//    }
//    else if(ENCODER1_A_INTERRUPT) //if P3.6 interrupted on A input, check input values
//    {
//        if(GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == 0)
//        {
//            g_encoder_state = Clockwise;
//            if(g_current_input == 0)
//            {
//                g_current_input = 5;
//            }
//        }
//        else
//        {
//            g_encoder_state = CounterClockwise;
//            if(g_current_input == 0)
//            {
//                g_current_input = 6;
//            }
//        }
//    }

    P3->IFG &= ~(BIT5 | BIT7); //reset interrupt flag for P3.6

}



//end of file
