/*
 * PushButton.c
 *
 *  Created on: Oct 11, 2022
 *      Author: James Tsai
 */
#include "PushButton.h"
void configurePushButton()
{
    //push button is on P1.5
    P1->DIR = 0x00;
    P1->REN |= 0x06;
    P1->OUT = 0x06; //set pull up resistor on P1.5
    P1->DIR = 0x01;
}



