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
void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	clearBoard();
	  printBoard();
	  hoverMove(g_current_x, g_current_y);
	  printBoard();
	  while (g_b_in_game) {
	    char move = getConsoleInput();
	    if (move == '\n')
	      continue;
	    moveHover(move);
	    printBoard();
	    printf("X: %i, Y: %i\n", g_current_x, g_current_y);
	  }
	  return 0;
}
//end of file
