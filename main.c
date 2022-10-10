#include "msp.h"
#include "board.h"
#include "moves.h"
#include "players.h"
#include <stdio.h>
#include <stdlib.h>

extern int g_current_x, g_current_y;
extern int g_b_in_game;

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    //initializing
    clear_board();
    print_board();
    init_players();
    hover_move(g_current_x, g_current_y);
    print_board();

    while (g_b_in_game) {
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
