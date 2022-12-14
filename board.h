/*
 * board.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef BOARD_H_
#define BOARD_H_


#include "defines.h"
#include "players.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void init_board();

uint8_t matrixrgb_write_pixel (uint16_t x, uint16_t y, uint8_t color );
uint8_t write_led_edge (uint16_t x, uint16_t y, uint16_t status );
uint8_t write_led_box (uint16_t x, uint16_t y, uint16_t status );
uint8_t write_led_dot (uint16_t x, uint16_t y, uint16_t status );
uint8_t write_player_score(struct Player player);
uint8_t write_digit(uint16_t start_y, int digit, int color);
void write_digit_to_led(uint16_t start_y, uint8_t score_matrix[5][3], int color);
void write_error(int error);
void write_winner(int state);
void clear_message();
void clear_bottom();




void write_initial_dots ();
void game_end();

#endif /* BOARD_H_ */
// end of file
