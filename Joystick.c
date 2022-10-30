/*
 * Joystick.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "Joystick.h"
#include "defines.h"
#include "msp.h"
#include "driverlib.h"


/*
 * ADC configuration sets the ADC to take input from the analog joystick, which is currently
 * set to pin 4.7 for y axis and 4.6 for x axis on the Joystick.
 * Converted values are stored in MEM0 and MEM1, which range between 0 and 16383 based on
 * potentiometer positions.
 *
 */

void configure_ADC()
{

    ADC14->CTL0 =  0x00000010;    // power on and disabled during configuration
    while(ADC14->CTL0 & ADC14_CTL0_BUSY){} //wait while adc busy
    ADC14->CTL0 = (ADC14_CTL0_SHP | ADC14_CTL0_SSEL__SMCLK | ADC14_CTL0_CONSEQ_1 | ADC14_CTL0_SHT1__32 |
            ADC14_CTL0_SHT0__32 | ADC14_CTL0_MSC | ADC14_CTL0_ON);
    // 31-30 ADC14PDIV  predivider,            00b = Predivide by 1
    // 29-27 ADC14SHSx  SHM source            000b = ADC14SC bit
    // 26    ADC14SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
    // 25    ADC14ISSH  invert sample-and-hold  0b = not inverted
    // 24-22 ADC14DIVx  clock divider         000b = /1
    // 21-19 ADC14SSELx clock source select   100b = SMCLK
    // 18-17 ADC14CONSEQx mode select          01b = Sequence-of-channels
    // 16    ADC14BUSY  ADC14 busy              0b (read only)
    // 15-12 ADC14SHT1x sample-and-hold time 0011b = 32 clocks
    // 11-8  ADC14SHT0x sample-and-hold time 0011b = 32 clocks
    // 7     ADC14MSC   multiple sample         1b = continue conversions automatically after first SHI signal trigger
    // 6-5   reserved                          00b (reserved)
    // 4     ADC14ON    ADC14 on                1b = powered up
    // 3-2   reserved                          00b (reserved)
    // 1     ADC14ENC   enable conversion       0b = ADC14 disabled
    // 0     ADC14SC    ADC14 start             0b = No start (yet)

    //start conversion address at MEM0 with 14bit resolution
    ADC14->CTL1 = (ADC14_CONVERSION_START_ADDR(0) | ADC14_CTL1_RES__14BIT);

    //map MEM0 to 4.6
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_6;

    //map MEM1 to 4.7 and set end of sequence
    ADC14->MCTL[1] = (ADC14_MCTLN_EOS |ADC14_MCTLN_INCH_7);

    // enable conversion on ADC
    ADC14->CTL0 |= ADC14_CTL0_ENC;
}

//end of file
