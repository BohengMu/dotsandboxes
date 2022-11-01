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
    check_ADC_state();
    if(InterruptCounter == 200){
        check_button_state();
        InterruptCounter = 0;
    }
    InterruptCounter++;
}


