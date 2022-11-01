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
    P1->IES |= BIT5; //interrupt on high to low transition
    P1->IFG = 0x00; //clear all interrupt flags
    P1->IE |= BIT5; //enable interrupt for P1.5
    Interrupt_enableInterrupt(INT_PORT1); //enable Port 1 interrupt on NVIC
    Interrupt_setPriority(INT_PORT1, 1); //set priority lower than encoder
}

/*
 * IRQ handler for port 1, which is triggered by the push button.
 * Handler will set button_pressed to 1 if interrupted.
 * Game logic will set button_pressed to 0 after move is authorized.
 *
 */
enum ButtonState check_button_state()
{
    /*
     * Poll routine for PushButton, which is polled every 2 ms.
     * Handler will set button_pressed to 1 if interrupted.
     * Game logic will set button_pressed to 0 after move is authorized.
     *
     */
    //if P1.5 is active low
    if(P1->IN & BIT5){
        g_button_state = Pressed;
    }
    else{
        g_button_state = NotPressed;
    }
    return g_button_state;
}


//end of file
