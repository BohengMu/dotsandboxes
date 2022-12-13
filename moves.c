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

//Whether a line is hovered
bool hover_line;

//which direction is the line pointing, -1 if uninitialized
int line_direction;

/*
 * Initialize game state, with zero lines selected and the dot at 0,0
 *
 */

void init_moves(){
    // start the game
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
    //select initial dot
    select_dot(0, 0);
}


/*
 * process the inputs given
 * can: move the dots, move the lines, select line
 * moves are based on g_current_input which is mapped as the following:
 * 1: joystick up
 * 2: joystick down
 * 3: joystick left
 * 4: joystick right
 * 5: encoder clockwise
 * 6: endocer counter clockwise
 * 7: button pressed
 */
void process_move()
{
  //boolean for when a player makes a mistake or completes a box
  bool b_can_move_again;

  //reset input state
  g_joystick_state = Zero;
  g_button_state = NotPressed;
  g_encoder_state = Neutral;

  //check if the game has ended
  if(!g_b_in_game)
  {
      //button pressed, resetting game
      if(g_current_input == 7)
      {
          reset_game();
      }
      //reset inputs
      g_current_input = 0;
      return;
  }

  //game has not ended proces moves in order of joystick, encoder, then button
  if(g_current_input == 1) // joystick up
  {
      clear_message();
      move_dot_selection(g_dot_x - 2, g_dot_y);

  }
  else if(g_current_input == 2)// joystick down
  {
      clear_message();
      move_dot_selection(g_dot_x + 2, g_dot_y);

  }
  else if(g_current_input == 3)// joystick left
  {
      clear_message();
      move_dot_selection(g_dot_x, g_dot_y - 2);

  }
  else if(g_current_input == 4) // joystick right
  {
      clear_message();
      move_dot_selection(g_dot_x, g_dot_y + 2);

  }
  else if(g_current_input == 5) // encoder clockwise
  {
      clear_message();
      move_line_selection(true);

  }
  else if(g_current_input == 6) // encoder counter clockwise
  {
      clear_message();
      move_line_selection(false);
  }
  else if(g_current_input == 7) //button pressed
  {
      clear_message();
      b_can_move_again = submit_selected_line();

      // change players of no boxes are formed
      if (!b_can_move_again)
      {
          current_player_id += 1;
          current_player_id %= 2;
      }

      //check if there is any more box left
      if(g_players[0].score + g_players[1].score >= ROWS * COLUMNS)
      {
          // clear messages to display winner
          clear_bottom();

          if(g_players[0].score > g_players[1].score)//p1 wins
          {
              write_winner(0);
          }
          else if(g_players[1].score > g_players[0].score)// p2 wins
          {
              write_winner(1);
          }
          else//tie
          {
              write_winner(2);
          }

          //indicate end of game
          end_game();

      }
      else{//write player score
          write_player_score(g_players[current_player_id]);
      }
  }
  // reset input queue
  g_current_input = 0;
  return;
}

/*
 * reset the game
 * which happens after all boxes are filled and the button is pressed
 * resets player score, boxes, edges, and any counters
 */
void reset_game()
{
    // clear messages
    clear_bottom();

    // clear led board
    refresh_led_board();

    //reset players
    init_players();

    //clear game board
    init_board();

    //clear move states
    init_moves();
}

/*
 * end game state when all boxes are filled
 */
void end_game()
{
    //indicate end of game, halt move processing
    g_b_in_game = 0;

    //add a delay to prevent double press of button
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

  // chec if the line can be selected
  if (line_direction == LINE_NOT_SELECTED)
  {
    write_error(2);
    return true;
  }

  // check if the move have been selected
  if (g_board[g_line_x][g_line_y] == MOVE_SELECTED)
  {
    write_error(1);
    return true;
  }

  // select the move in game state
  g_board[g_line_x][g_line_y] = MOVE_SELECTED;

  // reflect in led
  write_led_edge(g_line_x, g_line_y, g_board[g_line_x][g_line_y]);

  // check for adjacent boxes
  if (g_line_x % 2 == 1) // for boxes row
  {

    // check box right
    if (check_box(g_line_x, g_line_y + 1))
    {
      // update box in game state to the player that claimed the box
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
      // update box in game state to the player that claimed the box
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
      // update box in game state to the player that claimed the box
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
      // update box in game state to the player that claimed the box
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
      write_error(3);
    //printf("Cannot select move, out of bounds \n");
    return;
  } else {
    // remove current selections
    if (line_direction != LINE_NOT_SELECTED)
    {
      unselect_line();
      line_direction = LINE_NOT_SELECTED;
    }


    unselect_dot();

    // move the new selections
    select_dot(dot_x, dot_y);

    //update global position
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
  if (line_direction != LINE_NOT_SELECTED)
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

  //select the line and set update values
  select_line(new_line_x, new_line_y);

  //update global position
  g_line_x = new_line_x;
  g_line_y = new_line_y;
  return;
}

/*
 * checks of the move is within the bounds of the game state
 */
bool valid_move(int move_x, int move_y)
{
  bool valid = (move_x >= 0) && (move_x < GAME_STATE_ROWS) && (move_y >= 0) &&
               (move_y < GAME_STATE_COLUMNS);
  return valid;
}

/*
 * Checks if the box is completed at a given coordinate
 */
bool check_box(int box_x, int box_y)
{

  if (valid_move(box_x, box_y))//check if box is with in bounds
  {
    int count = 0;
    int direction;

    // check that lines in all 4 direction is selected
    for (direction = 0; direction < 4; direction++)
    {
      //check of an edge is selected
      if (g_board[box_x + x_change[direction]][box_y + y_change[direction]])
      {
          count++;
      }

    }
    if (count == 4)//all four edge selected, claim the box
    {
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
  g_board[dot_x][dot_y] = DOT_SELECTED;

  // reflect led
  write_led_dot(dot_x, dot_y, DOT_SELECTED);
}

/*
 * un-hover the dot for the current dot selection
 * this is to help the output to refelect the changes in selection
 */
void unselect_dot() {
  // reflect game state
  g_board[g_dot_x][g_dot_y] = DOT_NOT_SELECTED;

  // reflect led
  write_led_dot(g_dot_x, g_dot_y, DOT_NOT_SELECTED);
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
