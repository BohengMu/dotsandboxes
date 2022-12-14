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

//global variables that indicate current state of joystick and input queue to process game logic
volatile enum JoystickState g_joystick_state;
extern volatile int g_current_input;
/*
 * ADC configuration sets the ADC to take input from the analog joystick, which is currently
 * set to pin 5.1 for y axis and 5.0 for x axis on the Joystick.
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

    //map MEM0 to 5.1
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_4;

    //map MEM1 to 5.0 and set end of sequence
    ADC14->MCTL[1] = (ADC14_MCTLN_EOS |ADC14_MCTLN_INCH_5);

    // enable conversion on ADC
    ADC14->CTL0 |= ADC14_CTL0_ENC;
}

/*
 * Interrupt routine in InputInterrupt.c calls this function
 * If ADC conversion is done, determines what position Joystick is in
 * Return value is current state of Joystick
 * Input queue only adds move if this is the first move to be processed
 * Input Queue: g_current_input: Up is 1, Down is 2, Left is 3, Right is 4
 */
enum JoystickState check_ADC_state()
{
    if(!ADC14_CONVERSION_FINISHED) //if conversion is not done, stay in current state
    {
        g_joystick_state = Zero;
        return g_joystick_state;
    }
    else //otherwise, set joystick state based on ADC conversion values
    {
        if(JOYSTICK_RIGHT) //right is input queue value 4
        {
            g_joystick_state = Right;
            if(g_current_input == 0)
            {
                g_current_input = 4;
            }
        }
        else if(JOYSTICK_DOWN) //down is input queue value 2
        {
            g_joystick_state = Down;
            if(g_current_input == 0)
            {
                g_current_input = 2;
            }

        }
        else if(JOYSTICK_LEFT) //left is input queue value 3
        {
            g_joystick_state = Left;
            //since max ref voltage is 3.3 and we run on 5 V, there is issue of moving left registering a move right initially
            //so override the move right with move left
            if(g_current_input == 0 || g_current_input == 4)
            {
                g_current_input = 3;
            }

        }
        else if(JOYSTICK_UP) //up is input queue value 1
        {
            g_joystick_state = Up;
            if(g_current_input == 0)
            {
                g_current_input = 1;
            }
        }
        else //otherwise, the joystick is neutral
        {
            g_joystick_state = Zero;
        }

        ADC14_CONVERSION_START; //start another conversion
    }
    return g_joystick_state;
}

//end of file
