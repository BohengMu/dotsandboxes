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


/*
 * Push button is configured on pin 1.5 with pull up resistor.
 * Interrupts are triggered on falling edges
 *
 */
void configure_push_button()
{
    P1->SEL0 &= ~BIT5; //set P1 to I/O function
    P1->SEL1 &= ~BIT5; //set P1 to I/O function
    P1->DIR &= ~BIT5; //configure port 1.5 as input
    P1->REN |= BIT5; //enable pull up resistors on Port 1.5
    P1->OUT |= BIT5; //select pull up resistor on Port 1.5
    P1->IFG = 0x00; //clear all interrupt flags
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
    if(P1->IN & BIT5)
    {
        g_button_state = NotPressed;
        return g_button_state;
    }
    //if P1.5 is active low (pressed)
    if(!(P1->IN & BIT5))
    {
        g_button_state = Pressed;
        return g_button_state;
    }
    return g_button_state;

}


//end of file
