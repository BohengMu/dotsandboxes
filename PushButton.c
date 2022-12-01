/*
 * PushButton.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "PushButton.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "msp.h"
#include "driverlib.h"


volatile enum ButtonState g_button_state;
//extern vector g_input_vector;
extern volatile int g_current_input;
/*
 * Push button is configured on pin 4.7 with pull up resistor.
 * Interrupts are triggered on falling edges
 *
 */
void configure_push_button()
{
    P5->SEL0 &= ~BIT2; //set P4 to I/O function
    P5->SEL1 &= ~BIT2; //set P4 to I/O function
    P5->DIR &= ~BIT2; //configure port 4.7 as input
    P5->REN |= BIT2; //enable pull up resistors on Port 4.7
    P5->OUT |= BIT2; //select pull up resistor on Port 4.7
    P5->IFG = 0x00; //clear all interrupt flags
}

/*
 * Interrupt routine in InputInterrupt.c calls this function every 5 ms
 * Handler will set g_button_state to Pressed or NotPressed based on state.
 * Return value is current state of button.
 *
 */
enum ButtonState check_button_state()
{
    /*
     * Poll routine for PushButton, which is polled every 5 ms.
     * Handler will set button_pressed to 1 if button is pressed, otherwise 0
     *
     */
    //if P1.5 is active high (not pressed)
    if(P5->IN & BIT2)
    {
        g_button_state = NotPressed;
        return g_button_state;
    }

    //if P1.5 is active low (pressed)
    if(!(P5->IN & BIT2))
    {
        g_button_state = Pressed;
        if(g_current_input == 0)
        {
            g_current_input = 7;
        }
        //g_input_vector.pfVectorAdd(&g_input_vector, "pushed");
        //printf("ADDED 7\n");
        return g_button_state;
    }
    return g_button_state;

}


//end of file
