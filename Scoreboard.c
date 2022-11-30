/*
 * Scoreboard.c
 *
 *  Created on: Nov 18, 2022
 *      Author: James Tsai
 */

#include "Scoreboard.h"
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"


/*
 * The master input port/bits for the four 7-seg displays are on
 * SCLK1: P6.5
 * SCLK2: P1.7
 * SDA1: P6.4
 * SDA2: P1.6
 *
 */
void configure_scoreboard(void)
{
    P6->DIR &= ~(BIT4 | BIT5);
    P1->DIR &= ~(BIT6 | BIT7);
    P6->REN |= BIT4;
    P1->REN |= BIT6;

}


