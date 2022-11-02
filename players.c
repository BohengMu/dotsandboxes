/*
 * players.c
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#include "players.h"

struct Player player2;
struct Player player1;
volatile struct Player g_players[2];

/*
 * initialize the global players
 */
void init_players()
{
  player1.ID = 1;
  player2.ID = 2;
  player1.score = 0;
  player2.score = 0;
  player1.color = 0;
  player2.color = 0;
  g_players[0] = player1;
  g_players[1] = player2;
}

//end of file
