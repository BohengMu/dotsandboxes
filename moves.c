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

void hover_move(int i, int j)
{
    g_board[i][j] = (g_board[i][j] + 2);
}

void dehover_move(int i, int j)
{
    g_board[i][j] = (g_board[i][j] - 2);
}

void move_hover(char move)
{
  dehover_move(g_current_x, g_current_y);
  int success;
  switch (move) {

  case 'w':
    if (valid_move(g_current_x - 1, g_current_y))
      g_current_x--;
    hover_move(g_current_x, g_current_y);
    break;

  case 's':
    if (valid_move(g_current_x + 1, g_current_y))
      g_current_x++;
    hover_move(g_current_x, g_current_y);
    break;

  case 'a':
    if (valid_move(g_current_x, g_current_y - 1))
      g_current_y--;
    hover_move(g_current_x, g_current_y);
    break;

  case 'd':
    if (valid_move(g_current_x, g_current_y + 1))
      g_current_y++;
    hover_move(g_current_x, g_current_y);
    break;
  case 'q':
      g_b_in_game = 0;
    break;
  case 'l':
    success = select_move(g_current_x, g_current_y);
    if (success) {
      current_player_id++;
      current_player_id %= 2;
    }
    hover_move(g_current_x, g_current_y);
    break;
  }
}

char get_console_input() {
  char move;
  printf("Player 1 score: %i, Player 2 : %i \n", g_players[0].score,
         g_players[1].score);
  printf("select a move for player %i: ", g_players[current_player_id].ID);

  move = getchar();
  return move;
}

int select_move(int select_x, int select_y) {
  int move_again = 0;
  if ((select_x + select_y) % 2 == 0) {
    printf("Cannot select move, on a dot or box\n");
    return 0;
  } else {
    if (g_board[select_x][select_y] != 0) {
      printf("Cannot select a selected move!\n");
      hover_move(g_current_x, g_current_y);
      return 0;
    }
    g_board[select_x][select_y] = 1;
    // check for boxes
    if (select_x % 2 == 1) {
      if (check_box(select_x, select_y + 1) && g_board[select_x][select_y + 1] == 0) {
        g_board[select_x][select_y + 1] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        move_again++;
      }
      if (check_box(select_x, select_y - 1) && g_board[select_x][select_y - 1] == 0) {
          g_board[select_x][select_y - 1] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        move_again++;
      }
    } else {
      if (check_box(select_x + 1, select_y) && g_board[select_x + 1][select_y] == 0) {
        g_board[select_x + 1][select_y] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        move_again++;
      }
        g_board[select_x - 1][select_y] = g_players[current_player_id].ID;
        g_players[current_player_id].score++;
        move_again++;
      }
    }
    if (move_again)
    {
      return 0;
    }
    return 1;
}


int valid_move(int move_x, int move_y) {
  int valid = (move_x >= 0) && (move_x < ROWS * 2 + 1 && move_y >= 0) && (move_y < COLUMNS * 2 + 1);
  if (!valid)
    printf("Out of Bounds!\n");
  return valid;
}

int check_box(int box_x, int box_y) {
  if (valid_move(box_x, box_y)) {
    int count = 0;
    int direction;
    for (direction = 0; direction < 4; direction++) {
      if (g_board[box_x + x_change[direction]][box_y + y_change[direction]])
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
