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
#include "msp.h"
#include "InputInterrupt.h"
#include "RotaryEncoder.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

//counter for number of interrupts occurred
volatile int InterruptCounter = 0;
//set current input to no input value
/*
 * configure SysTick to trigger at a period of 48000
 * Since Clock is 48MHz, interrupts occur every 1 ms
 */
void configure_systick(void)
{
    //configures adc to read in joystick input, see Joystick.c
    configure_ADC();

    //with a period of 48000, the interrupt is at a period od
    // 48MHz /48000 = 1 ms
    SysTick_setPeriod(48000);

    //start modules
    SysTick_enableInterrupt();
    Interrupt_enableMaster();
    ADC14_CONVERSION_START; //start an ADC conversion
    SysTick_enableModule();
}

/*
 * Interrupt Handler for SysTick, runs every 1 ms
 * checks Joystick values every 1 ms and PushButton value every 5 ms
 */
void SysTick_Handler(void)
{
    check_ADC_state(); //check joystick values, see Joystick.c
    if(InterruptCounter == 5){ //on every 5th interrupt (5 ms)
        check_button_state(); //check if button is pressed
        //check_encoder_state(); //check if encoder is moved
        InterruptCounter = 0; //reset InterruptCounter
    }
    InterruptCounter++;
}
