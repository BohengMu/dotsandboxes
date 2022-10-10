/*
 * moves.c
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */


#include "board.h"
#include "defines.h"
#include "players.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "moves.h"

int x_change[4] = {1, -1, 0, 0};
int y_change[4] = {0, 0, 1, -1};
extern struct Player g_players[2];


extern int g_board[ROWS * 2 + 1][COLUMNS * 2 + 1];
int g_b_in_game = 1;
int g_current_x = 0, g_current_y = 0;
int current_player_id = 0;

void hoverMove(int i, int j)
{
    g_board[i][j] = (g_board[i][j] + 2);
}

void dehoverMove(int i, int j)
{
    g_board[i][j] = (g_board[i][j] - 2);
}

void moveHover(char move)
{
  dehoverMove(g_current_x, g_current_y);
  int success;
  switch (move) {

  case 'w':
    if (validMove(g_current_x - 1, g_current_y))
      g_current_x--;
    hoverMove(g_current_x, g_current_y);
    break;

  case 's':
    if (validMove(g_current_x + 1, g_current_y))
      g_current_x++;
    hoverMove(g_current_x, g_current_y);
    break;

  case 'a':
    if (validMove(g_current_x, g_current_y - 1))
      g_current_y--;
    hoverMove(g_current_x, g_current_y);
    break;

  case 'd':
    if (validMove(g_current_x, g_current_y + 1))
      g_current_y++;
    hoverMove(g_current_x, g_current_y);
    break;
  case 'q':
      g_b_in_game = 0;
    break;
  case 'l':
    success = selectMove(g_current_x, g_current_y);
    if (success) {
      current_player_id++;
      current_player_id %= 2;
    }
    hoverMove(g_current_x, g_current_y);
    break;
  }
}

char getConsoleInput() {
  char move;
  printf("Player 1 score: %i, Player 2 : %i \n", g_players[0].score,
         g_players[1].score);
  printf("select a move for player %i: ", g_players[current_player_id].ID);

  move = getchar();
  return move;
}

int selectMove(int i, int j) {
  int moveAgain = 0;
  if ((i + j) % 2 == 0) {
    printf("Cannot select move, on a dot or box\n");
    return 0;
  } else {
    if (g_board[i][j] != 0) {
      printf("Cannot select a selected move!\n");
      hoverMove(g_current_x, g_current_y);
      return 0;
    }
    g_board[i][j] = 1;
    // check for boxes
    if (i % 2 == 1) {
      if (checkBox(i, j + 1) && g_board[i][j + 1] == 0) {
        board[i][j + 1] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        moveAgain++;
      }
      if (checkBox(i, j - 1) && g_board[i][j - 1] == 0) {
        board[i][j - 1] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        moveAgain++;
      }
    } else {
      if (checkBox(i + 1, j) && g_board[i + 1][j] == 0) {
        g_board[i + 1][j] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        moveAgain++;
      }
      if (checkBox(i - 1, j) && board[i - 1][j] == 0) {
        g_board[i - 1][j] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        moveAgain++;
      }
    }
    if (moveAgain)
      return 0;
    return 1;
  }
}

int validMove(int i, int j) {
  int valid = i >= 0 && i < ROWS * 2 + 1 && j >= 0 && j < COLUMNS * 2 + 1;
  if (!valid)
    printf("Out of Bounds!\n");
  return valid;
}

int checkBox(int i, int j) {
  if (validMove(i, j)) {
    int count = 0;
    int k;
    for (k = 0; k < 4; k++) {
      if (g_board[i + x_change[k]][j + y_change[k]])
        count++;
    }
    if (count == 4) {
      printf("!!!!!!!!!!!!!got a box!!!!!!!!!!!!!\n");
      return 1;
    }
  }
  return 0;
}

//end of file
