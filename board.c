/*
 * board.c
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */


#include "board.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>

int g_board[ROWS * 2 + 1][COLUMNS * 2 + 1];

char verticle_edges[4] = {' ', '-', '^', 'x'};
char horizontal_edges[4] = {' ', '|', '>', 'x'};
char dots[3] = {'.', ' ', 'o'};
char boxes[5] = {' ', '1', '2', 'x', 'x'};

/*
 * Reconfigure the board to all zeros
 */
void clear_board()
{
  int i, j;
  for (i = 0; i < 2 * ROWS + 1; i++)
  {
    for (j = 0; j < 2 * COLUMNS + 1; j++)
    {
        g_board[i][j] = 0;
    }
  }
}


/*
 * Print the current board
 * TODO: make this work with the LED matrix
 */
void print_board()
{
  printf("--------------boad of size %i x %i--------------\n", ROWS, COLUMNS);
  int row, column;
  for (row = 0; row < 2 * ROWS; row += 2)
  {
    char dotrow[2 * COLUMNS + 2];
    char boxrow[2 * COLUMNS + 2];
    dotrow[2 * COLUMNS + 1] = '\0';
    boxrow[2 * COLUMNS + 1] = '\0';
    for (column = 0; column < 2 * COLUMNS + 1; column++) {
      dotrow[column] = (column % 2 == 0) ? dots[g_board[row][column]] : verticle_edges[g_board[row][column]];
      boxrow[column] =
          (column % 2 == 0) ? horizontal_edges[g_board[row + 1][column]] : boxes[g_board[row][column]];
    }
    printf("%s\n%s\n", dotrow, boxrow);
  }
  char dotrow[2 * COLUMNS + 2];
  dotrow[2 * COLUMNS + 1] = '\0';

  //last column
  for (column = 0; column < 2 * COLUMNS + 1; column++)
  {
    dotrow[column] =
        (column % 2 == 0) ? dots[g_board[2 * ROWS][column]] : verticle_edges[g_board[2 * ROWS][column]];
  }
  printf("%s\n", dotrow);
}

//end of file
