/*
 * InputInterrupt.c
 *
 *  Created on: Nov 1, 2022
 *      Author: James Tsai
 */
#include "PushButton.h"
#include "Joystick.h"
#include "driverlib.h"
#include "systick.h"
#include "InputInterrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

volatile enum JoystickState g_joystick_state = Zero;
volatile enum ButtonState g_button_state = NotPressed;
volatile int InterruptCounter = 0;
/*
 * configure SysTick to trigger at a period of 30
 * Since MCLK is 3MHz, interrupts occur every 10 us
 */

void configure_systick(void)
{
    //configures adc to read in joystick input, see Joystick.c
    configure_ADC();
    SysTick_enableModule();
    SysTick_setPeriod(30);
    SysTick_enableInterrupt();
    Interrupt_enableMaster();
}

void SysTick_Handler(void)
{
    if(!ADC14_CONVERSION_FINISHED)
    {
        g_joystick_state = Zero;
    }
    else
    {
        if(JOYSTICK_UP)
        {
            g_joystick_state = Up;
        }
        else if(JOYSTICK_DOWN)
        {
            g_joystick_state = Down;
        }
        else if(JOYSTICK_LEFT)
        {
            g_joystick_state = Left;
        }
        else if(JOYSTICK_RIGHT)
        {
            g_joystick_state = Right;
        }
        else
        {
            g_joystick_state = Zero;
        }

        ADC14_CONVERSION_START;
    }
    if(InterruptCounter == 200){
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
                InterruptCounter = 0;
            }
            InterruptCounter++;


}


