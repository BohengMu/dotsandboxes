/*
 * main.c
 *
 *  Created on: Oct 25, 2022
<<<<<<< HEAD
 *      Author: mub91
 */

#include "msp.h"
#include "board.h"
#include "moves.h"
#include "players.h"
#include "RotaryEncoder.h"
#include "matrixdriver.h"
#include "PushButton.h"
#include "matrixdriver.h"
#include "Joystick.h"
#include "Clock.h"
#include "spi.h"
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "InputInterrupt.h"


/**
 * main.c runs all the initialization code for user inputs and game simulation.
 * while loop in main continuously runs ADC conversions
 *
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //initializing taken out for now to test Boheng's code
    configure_systick();
    configure_push_button();
    Clock_Init48MHz();
    //initializing game simulation and user inputs


    //configure encoder input, see RotaryEncoder.c
    configure_encoder();
    init_matrix();
    //initialize player values
    init_players();
    init_board();
    init_moves();
    //set dots to selcted and everyting else to zero



    //select top left dot as begining move


    //print initial game state
    int i;
    while (true) {
        //get input as from keyboard as a simulation
        // will be done automatically in the back ground
        //char move = get_console_input();

        // process the current input state
        process_move();

        for(i= 0 ; i < 150; i++){
            refresh_led_board();
        }
    }
    return;
}
