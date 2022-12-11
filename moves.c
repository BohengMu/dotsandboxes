/*
 * moves.c
 *
 *  Created on: Oct 10, 2022
 *      Author: mub91
 */

#include "moves.h"
#include "board.h"
#include "Clock.h"
#include "defines.h"
#include "players.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RotaryEncoder.h"
#include "PushButton.h"
#include "Joystick.h"
#include "matrixdriver.h"

//input states
extern volatile enum EncoderState g_encoder_state;
extern volatile enum ButtonState g_button_state;
extern volatile enum JoystickState g_joystick_state;
//extern vector g_input_vector;
extern volatile int g_current_input;
//player infomation
extern volatile struct Player g_players[2];

//game state board
extern volatile int g_board[ROWS * 2 + 1][COLUMNS * 2 + 1];

//led representation
extern volatile int led_matrix[LED_MATRIX_ROWS][LED_MATRIX_COLUMNS];

//x, y changes for iteration
int y_change[4] = {0, 1, 0, -1};
int x_change[4] = {-1, 0, 1, 0};

//global values
int g_b_in_game;
int g_dot_x, g_dot_y;
int g_line_x, g_line_y;

// who is currently moving
volatile int current_player_id;

//wheter a line is hovered
bool hover_line;

//which direction is the line pointing
int line_direction;

/*
 * prints player score and the
 * this is needed in the earlier stages of the game
 * to simulate seven segment output
 */

void init_moves(){
    g_b_in_game = 1;

    //set initial dots
    g_dot_x = 0;
    g_dot_y = 0;
    g_line_x = 0;
    g_line_y = 0;

    //set no line selection
    hover_line = 0;
    line_direction = -1;

    //player 1 goes first
    current_player_id = 0;

    select_dot(0, 0);

#define  PRESET_SIZE  20
    uint16_t preset_x[PRESET_SIZE] = {1,4,5,7,5,8,5,6,2,4,6,2, 1, 0, 1, 4, 8, 8};
    uint16_t preset_y[PRESET_SIZE] = {2,5,3,2,10,3,2,7,5,9,1,9, 6, 3, 10, 7, 5, 9};
    int i;
    for (i = 0; i<PRESET_SIZE; i++)
    {
        g_board[preset_x[i]][preset_y[i]] = 1;

              // reflect in led
        write_led_edge(preset_x[i], preset_y[i], 1);
    }

}

static void print_score(){
    printf("Player 1 score: %i, Player 2 Score: %i \n", g_players[0].score, g_players[1].score);
    printf("Player %i turn \n", current_player_id + 1);
}

/*
 * gets the console input
 * this function is needed in the earlier stages of the game
 * to simulate input signals on the terminal
 */
char get_console_input()
{
  char move;

  move = getchar();

  //repeat until a valid caracter is grapped
  while (move == '\n')
  {
    move = getchar();
  }
  return move;
}

/*
 * process the inputs given
 * can: move the dots, move the lines, select line
 * TODO: add logic to detect end of game
 */
void process_move(char move)
{
  //boolean for when a player makes a mistake or completes a box

  //TODO: take a snapshot of the
  bool b_can_move_again;

  //take a snapshot
  enum EncoderState encoder_state_copy = g_encoder_state;
  enum JoystickState joystick_state_copy = g_joystick_state;
  enum ButtonState button_pressed_copy = g_button_state;

  //reset input state
  g_joystick_state = Zero;
  g_button_state = NotPressed;
  g_encoder_state = Neutral;

  //process joystic input

  if(!g_b_in_game){
      if(g_current_input == 7){
          reset_game();
      }
      g_current_input = 0;
      return;
  }

  if(g_current_input == 1)
  {
      //printf("JOYSTICK UP");
      move_dot_selection(g_dot_x - 2, g_dot_y);

  }
  else if(g_current_input == 2)
  {
      //printf("JOYSTICK DOWN");
      move_dot_selection(g_dot_x + 2, g_dot_y);

  }
  else if(g_current_input == 3)
  {
      //printf("JOYSTICK LEFT");
      move_dot_selection(g_dot_x, g_dot_y - 2);

  }
  else if(g_current_input == 4)
  {
      //printf("JOYSTICK RIGHT");
      move_dot_selection(g_dot_x, g_dot_y + 2);

  }
  else if(g_current_input == 5)
  {
      //printf("ENCODER CLOCKWISE");
      move_line_selection(true);

      g_encoder_state = Neutral;
  }
  else if(g_current_input == 6)
  {
      //printf("ENCODER COUNTERCLOCKWISE");
      move_line_selection(false);

      g_encoder_state = Neutral;
  }
  else if(g_current_input == 7)
  {
      //printf("BUTTON PRESSED");
      b_can_move_again = submit_selected_line();

      // change players of no boxes are formed
      if (!b_can_move_again) {
          current_player_id += 1;
          current_player_id %= 2;
      }

      //check if there is any more box left
      if(g_players[0].score + g_players[1].score >= ROWS * COLUMNS){
          g_b_in_game = 0;

      }
      //g_button_pressed = 0;
      write_player_score(g_players[current_player_id]);
  }
  g_current_input = 0;
  return;
}

//end game procedure, no more boxes left
void reset_game(){

    init_players();
    init_board();
    init_moves();
}

//end game procedure, no more boxes left
void end_game(){
    g_b_in_game = 0;
    int i;
    for(i= 0 ; i < 150; i++){
          refresh_led_board();
      }
}

/*
 * submit the hovered line
 */
bool submit_selected_line()
{
  // check if the line is valid
  bool b_can_move_again = false;

  // chech if the line can be selected
  if (line_direction == -1)
  {
    //printf("Cannot select move, no line is selected\n");
    return true;
  }

  // check if the move have been selected
  if (g_board[g_line_x][g_line_y] == 3)
  {
    //printf("Cannot select a selected move!\n");
    return true;
  }

  // select the move in game state
  g_board[g_line_x][g_line_y] = 3;

  // reflect in led
  write_led_edge(g_line_x, g_line_y, g_board[g_line_x][g_line_y]);

  // check for adjacent boxes
  if (g_line_x % 2 == 1) // for boxes row
  {

    // check box right
    if (check_box(g_line_x, g_line_y + 1))
    {
      // update box in game state
      g_board[g_line_x][g_line_y + 1] = g_players[current_player_id].ID;

      // update in led
      write_led_box(g_line_x, g_line_y + 1, g_board[g_line_x][g_line_y + 1]);

      // update score
      g_players[current_player_id].score++;
      b_can_move_again = true;
    }

    // check box left
    if (check_box(g_line_x, g_line_y - 1))
    {
      // update box in game state
      g_board[g_line_x][g_line_y - 1] = g_players[current_player_id].ID;

      // update in led
      write_led_box(g_line_x, g_line_y - 1, g_board[g_line_x][g_line_y - 1]);

      // update score
      g_players[current_player_id].score++;
      b_can_move_again = true;
    }
  } else // for dots row
  {

    // check for box below
    if (check_box(g_line_x + 1, g_line_y))
    {
      // update box in game state
      g_board[g_line_x + 1][g_line_y] = g_players[current_player_id].ID;

      // update in led
      write_led_box(g_line_x + 1, g_line_y, g_board[g_line_x + 1][g_line_y]);

      // update score
      g_players[current_player_id].score++;
      b_can_move_again = true;
    }

    // check for box above
    if (check_box(g_line_x - 1, g_line_y))
    {
      // update box in game state
      g_board[g_line_x - 1][g_line_y] = g_players[current_player_id].ID;

      // update in led
      write_led_box(g_line_x - 1, g_line_y, g_board[g_line_x - 1][g_line_y]);

      // update score
      g_players[current_player_id].score++;
      b_can_move_again = true;
    }
  }

  // check if player gets to move again
  if (b_can_move_again)
  {
    return true;
  }
  return false;
}

/*
 * attempt to move the dot selection
 * changes the game state and the output
 */
void move_dot_selection(int dot_x, int dot_y)
{
  // check if dot is within bounds
  if (!valid_move(dot_x, dot_y)) {
    //printf("Cannot select move, out of bounds \n");
    return;
  } else {
    // remove current selections
    if (line_direction != -1)
    {
      unselect_line();
      line_direction = -1;
    }
    unselect_dot();

    // move the new selections
    select_dot(dot_x, dot_y);
    g_dot_x = dot_x;
    g_dot_y = dot_y;
  }
  return;
}

/*
 * move the line selection around the current dot
 * in the clock wise or counter clock wise direction
 */
void move_line_selection(bool b_is_clockwise)
{
  // unselect line, if possible
  if (line_direction != -1)
  {
    unselect_line();
  } else { // if no line is selected, select one first
    line_direction = 0;
  }
  int new_line_x, new_line_y;

  // pick the next available line
  do {
    // add 1 if is clocwise, subtract 1 if counter clockwise
    if (b_is_clockwise)
    {
      //increment clockwise
      line_direction += 1;

      //cycle
      line_direction %= 4;
    } else {
      //decrement counter clockwise
      line_direction -= 1;

      //c does not do negative mod very well
      if (line_direction < 0)
      {
        line_direction += 4;
      }

      //cycle
      line_direction %= 4;
    }

    //determine line selected
    new_line_x = g_dot_x + x_change[line_direction];
    new_line_y = g_dot_y + y_change[line_direction];
  } while (!valid_move(new_line_x, new_line_y));

  //seletect the line and set update values
  select_line(new_line_x, new_line_y);
  g_line_x = new_line_x;
  g_line_y = new_line_y;
  return;
}

/*
 * checks of the move is within the bounds of the game state
 */
bool valid_move(int move_x, int move_y)
{
  bool valid = (move_x >= 0) && (move_x < ROWS * 2 + 1) && (move_y >= 0) &&
               (move_y < COLUMNS * 2 + 1);
  return valid;
}

/*
 * Checks if the box is completed
 */
bool check_box(int box_x, int box_y)
{
  if (valid_move(box_x, box_y))
  {
    int count = 0;
    int direction;

    // check that lines in all 4 direction is selected
    for (direction = 0; direction < 4; direction++)
    {
      if (g_board[box_x + x_change[direction]][box_y + y_change[direction]])
        count++;
    }
    if (count == 4)
    {
      //printf("!!!!!!!!!!!!!got a box!!!!!!!!!!!!!\n");
      return true;
    }
  }
  return false;
}

/*
 * change the dot from neutral state to selected state based on input (x, y)
 * this is to help the output to refelect the changes in selection
 */
void select_dot(int dot_x, int dot_y)
{
  // reflect game state
  g_board[dot_x][dot_y] = 1;

  // reflect led
  write_led_dot(dot_x, dot_y, 1);
}

/*
 * un-hover the dot for the current dot selection
 * this is to help the output to refelect the changes in selection
 */
void unselect_dot() {
  // reflect game state
  g_board[g_dot_x][g_dot_y] = 0;

  // reflect led
  write_led_dot(g_dot_x, g_dot_y, 0);
}

/*
 * change a line from neutral state to selected based on input (x, y)
 * this is to help the output to refelect the changes in selection
 */
void select_line(int line_x, int line_y) {
  g_board[line_x][line_y] = g_board[line_x][line_y] + 2;

  // reflect led
  write_led_edge(line_x, line_y, g_board[line_x][line_y]);
}

/*
 * unselect the current line
 * this is to help the output to refelect the changes in selection
 */
void unselect_line() {
  // reflect game state
  g_board[g_line_x][g_line_y] = g_board[g_line_x][g_line_y] - 2;

  // reflect led
  write_led_edge(g_line_x, g_line_y, g_board[g_line_x][g_line_y]);
}

// end of file
