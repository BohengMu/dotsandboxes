/*
 * matrixdrive.h
 *
 *  Created on: Nov 13, 2022
 *      Author: mub91
 */

#ifndef MATRIXDRIVER_H_
#define MATRIXDRIVER_H_
#include "stdint.h"
#define A_SELECT_MASK   0x01
#define B_SELECT_MASK   0x02
#define C_SELECT_MASK   0x04
#define D_SELECT_MASK   0x08
#define RED_MASK        0x04
#define GREEN_MASK      0x02
#define BLUE_MASK       0x01

void init_matrix();
void select_row(uint16_t select);
void set_rgb_data(uint8_t rgb1, uint8_t rgb2);
void latch_on();
void latch_off();
void toggle_clock();
void refresh_board();
void board_off();


#endif /* MATRIXDRIVER_H_ */
