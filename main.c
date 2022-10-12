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
#include <stdio.h>
#include <stdlib.h>

extern int g_current_x, g_current_y;
extern int g_b_in_game;
extern int16_t encoder_count;

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    //initializing
    configure_ADC();
    configure_encoder();
    configure_push_button();
    clear_board();
    print_board();
    init_players();
    hover_move(g_current_x, g_current_y);
    print_board();
    //LEDmatrixrgb_init();

    while (g_b_in_game) {
        //ADC14_CONVERSION_START; //begin ADC conversion
        //while(!(ADC14_CONVERSION_FINISHED)); //wait for conversion to finish

        if(!(PUSH_INPUT & PUSH_PIN))
        { //if push button is pressed

        }
        if(encoder_count > 0)
        { //if encoder is rotated clockwise

        }
        else
        { //if encoder is rotated counterclockwise

        }

        char move = get_console_input();
        if (move == '\n')
        {
            continue;
        }
        move_selection(move);
        print_board();
        printf("X: %i, Y: %i\n", g_current_x, g_current_y);
        }
    return;
}
//end of file
