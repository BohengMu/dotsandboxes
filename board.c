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

char vedges[4] = {' ', '-', '^', 'x'};
char hedges[4] = {' ', '|', '>', 'x'};
char dots[3] = {'.', ' ', 'o'};
char boxes[5] = {' ', '1', '2', 'x', 'x'};


void clearBoard() {
  int i, j;
  for (i = 0; i < 2 * ROWS + 1; i++) {
    for (j = 0; j < 2 * COLUMNS + 1; j++) {
        g_board[i][j] = 0;
    }
  }
}

void printBoard() {
  printf("--------------boad of size %i x %i--------------\n", ROWS, COLUMNS);
  int i, j;
  for (i = 0; i < 2 * ROWS; i += 2) {
    char dotrow[2 * COLUMNS + 2];
    char boxrow[2 * COLUMNS + 2];
    dotrow[2 * COLUMNS + 1] = '\0';
    boxrow[2 * COLUMNS + 1] = '\0';
    for (j = 0; j < 2 * COLUMNS + 1; j++) {
      dotrow[j] = (j % 2 == 0) ? dots[g_board[i][j]] : vedges[g_board[i][j]];
      boxrow[j] =
          (j % 2 == 0) ? hedges[g_board[i + 1][j]] : boxes[g_board[i + 1][j]];
    }
    printf("%s\n%s\n", dotrow, boxrow);
  }
  char dotrow[2 * COLUMNS + 2];
  dotrow[2 * COLUMNS + 1] = '\0';
  for (j = 0; j < 2 * COLUMNS + 1; j++)
    dotrow[j] =
        (j % 2 == 0) ? dots[g_board[2 * ROWS][j]] : vedges[g_board[2 * ROWS][j]];
  printf("%s\n", dotrow);
}

//end of file
