/*
 * matrixdrive.h
 *
 *  Created on: Nov 13, 2022
 *      Author: mub91
 */

#ifndef MATRIXDRIVER_H_
#define MATRIXDRIVER_H_
#include "stdint.h"

void init_matrix();
void select_row(uint16_t select);
void set_rgb_data(uint8_t rgb1, uint8_t rgb2);
void refresh_led_board();

#endif /* MATRIXDRIVER_H_ */
//end of file
