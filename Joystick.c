/*
 * Joystick.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "Joystick.h"

#include "msp.h"
#include "driverlib.h"


/*
 * configure the adc to take input from the analog joystick, which is currently
 * set to pin 5.5 and 5.4
 */
void configureADC()
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0);

    //set sample mode for ADC buffer 0 and 1, store conversion results in MEM0 and MEM1
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureSingleSampleMode(ADC_MEM1, true);

    //configuring 32 registers for ADC, range is 2.5 V, ADC_INPUT_A0 = pin 5.5, ADC_INPUT_A1 = pin 5.4
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false);
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();
}


