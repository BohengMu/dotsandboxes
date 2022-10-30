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
#include "PushButton.h"
#include "Joystick.h"
#include "rgbmatrix.h"
#include "Clock.h"
#include "spi.h"
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * main.c runs all the initialization code for user inputs and game simulation.
 * while loop in main continuously runs ADC conversions
 *
 */


void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //initializing taken out for now to test Boheng's code
    //configure_ADC();
    //configure_encoder();
    //configure_push_button();

    //initializing game simulation and user inputs
    //configures adc to read in joystick input, see Joystick.c
    configure_ADC();

    //configure encoder input, see RotaryEncoder.c
    configure_encoder();

    //initialize player values
    init_players();

    //set dots to selcted and everyting else to zero
    clear_board();

    //write initial dots to led
    write_initial_dots();

    //select top left dot as begining move
    select_dot(0, 0);

    //print initial game state
    print_board();

    while (true) {
        //get input as from keyboard as a simulation
        // will be done automatically in the back ground
        char move = get_console_input();

        // process the current input state
        process_move(move);

        // update the output state
        print_board();
    }
    return;
}

//end of file
