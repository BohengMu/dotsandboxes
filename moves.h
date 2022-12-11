/*
 * moves.h
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#ifndef MOVES_H_
#define MOVES_H_
#include<stdbool.h>

char get_console_input();
void init_moves();

void process_move(char move);
bool submit_selected_line();

void move_dot_selection(int dot_x, int dot_y);
void move_line_selection(bool b_is_clockwise);

bool valid_move(int move_x, int move_y);
bool check_box(int box_x, int box_y);

void select_dot(int dot_x, int dot_y);
void unselect_dot();
void select_line(int line_x, int line_y);
void unselect_line();
void end_game();
void reset_game();

#endif /* MOVES_H_ */
// end of file
