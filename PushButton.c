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

//global variables that indicate current state of button and input queue to process game logic
volatile enum ButtonState g_button_state;
extern volatile int g_current_input;
/*
 * Push button is configured on pin 5.2 with pull up resistor.
 * Interrupts are triggered on falling edges
 *
 */
void configure_push_button()
{
    P5->SEL0 &= ~BIT2; //set P5 to I/O function
    P5->SEL1 &= ~BIT2; //set P5 to I/O function
    P5->DIR &= ~BIT2; //configure port 5.2 as input
    P5->REN |= BIT2; //enable pull up resistors on Port 5.2
    P5->OUT |= BIT2; //select pull up resistor on Port 5.2
    P5->IFG = 0x00; //clear all interrupt flags
}

/*
 * Interrupt routine in InputInterrupt.c calls this function every 5 ms
 * Handler will set g_button_state to Pressed or NotPressed based on state.
 * Return value is current state of button.
 * Input Queue only adds move if this is the first move to be processed
 * Input Queue: Set g_current_input to 7 if pressed
 */
enum ButtonState check_button_state()
{
    /*
     * Poll routine for PushButton, which is polled every 5 ms.
     * Handler will set button_pressed to 1 if button is pressed, otherwise 0
     *
     */
    //if P5.2 is active high (not pressed), update button state
    if(P5->IN & BIT2)
    {
        g_button_state = NotPressed;
        return g_button_state;
    }

    //if P5.2 is active low (pressed), update button state
    if(!(P5->IN & BIT2))
    {
        g_button_state = Pressed;
        if(g_current_input == 0) //if this is the first move to be processed, add to input queue
        {
            g_current_input = 7;
        }
        return g_button_state;
    }
    return g_button_state;

}

//end of file
