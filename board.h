/*
 * board.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef BOARD_H_
#define BOARD_H_


#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void clear_board();
void print_board();

uint8_t mock_matrixrgb_write_pixel ( int led_matrix[LED_MATRIX_SIZE][LED_MATRIX_SIZE], uint16_t x, uint16_t y, uint16_t color );
uint8_t write_led_edge (uint16_t x, uint16_t y, uint16_t status );
uint8_t write_led_box (uint16_t x, uint16_t y, uint16_t status );
uint8_t write_led_dot (uint16_t x, uint16_t y, uint16_t status );
void write_initial_dots ();


#endif /* BOARD_H_ */
// end of file
