/*
   * board.c
   *
   *  Created on: Oct 10, 2022
   *      Author: mub91
   */

 #include "board.h"
 #include "defines.h"
 #include <stdint.h>
 #include <stdio.h>
 #include <stdlib.h>

 //game state board
 volatile int g_board[ROWS * 2 + 1][COLUMNS * 2 + 1];

 //define game state board size
 int box_size;
 int inner_box_size;

 //led board
 volatile uint8_t g_led_matrix[LED_MATRIX_SIZE][LED_MATRIX_SIZE];

 /*
  * initialize the game state board and the led board which does:
  * Reconfigure the game board to all zeros
  * reconfigure the led board to all zeros
  * write necessary word and pixels
  */
 void init_board()
 {
     //compute game state size based on rows desired and led size
     int row_size = ((LED_MATRIX_ROWS-1)/ROWS +1);
     int column_size = ((LED_MATRIX_COLUMNS-1)/COLUMNS +1);

     //compute box_size
     if (row_size < column_size)
     {
         box_size = row_size;
     }
     else
     {
         box_size = column_size;
     }
     inner_box_size = box_size - 2;

     //set led to all zeros
     memset(g_led_matrix, 0, LED_MATRIX_ROWS * LED_MATRIX_COLUMNS * sizeof(uint8_t));

     //set all game state to zeros
     int i, j;
     for (i = 0; i < 2 * ROWS + 1; i++) {
         for (j = 0; j < 2 * COLUMNS + 1; j++) {
             g_board[i][j] = 0;
         }
     }

    //write all the game dots
    for(i = 0; i < ROWS + 1; i ++){
        for(j = 0;j< COLUMNS + 1; j ++){
            write_led_dot(i*2, j*2, 0);
        }
    }

    //write the p at the botton
    uint8_t p_matrix[5][3] =
      {
       {1, 1, 0},
       {1, 0, 1},
       {1, 1, 0},
       {1, 0, 0},
       {1, 0, 0},
      };
    uint16_t x, y;
    for (x = 27; x < 32; x++){
        matrixrgb_write_pixel(x, 4, 1);
        for (y = 0; y < 3; y++){
            matrixrgb_write_pixel(x, y, p_matrix[x-27][y]);
        }
    }

    //write the semicolon
    matrixrgb_write_pixel(28, 8, 1);
    matrixrgb_write_pixel(30, 8, 1);

    // write the score to 00 with the color blue
    write_digit(10, 0, BLUE);
    write_digit(14, 0, BLUE);

}

  /*
   * a simpulation of Josephs matrixrgb_write_pixel function, it has the same
   * interface except the matrixrgb_t  is switched with a 32x32 led matrix this
   * function writes a single pixel at coordinates x and y with color the mock
   * function will represents the color with 0- off, 1 - dot color 1, 2 - dot color
   * 2, 3 - played edge, 4 - available edge, 5 - unavailable, edge 6 - player 1 box, 7
   * - player 2 box
   */
  uint8_t matrixrgb_write_pixel(uint16_t x, uint16_t y,
                                uint8_t color)
  {

    if ((x >= LED_MATRIX_SIZE) || (y >= LED_MATRIX_SIZE))
    {
      return 1;
    }
    g_led_matrix[x][y] = color;
    return 0;
  }

  /*
   * write the edge onto the led matrix
   * edge could be horizontal or vertiacal
   * edge could also be choosen for not chosen
   *
   */
  uint8_t write_led_edge(uint16_t x, uint16_t y, uint16_t status)
  {
    // determining color
    uint8_t color = status + 2;

    // unselected edge should be off
    if (status == 0)
    {
      color = BLACK;
    }

    // determine if edge is horizontal or vertical
    if (x % 2 == 0) // horizontal edge
    {
      // projecting game coordinate to led coordinate
      uint16_t edge_base_x = ((x / 2) * (inner_box_size + 1));
      uint16_t edge_base_y = 1 + (((y - 1) / 2) * (inner_box_size + 1));

      // print the edge on the led
      uint16_t edge_offset;
      for (edge_offset = 0; edge_offset < inner_box_size; edge_offset++)
      {
        matrixrgb_write_pixel(edge_base_x,
                                   edge_base_y + edge_offset, color);
      }
      return 0;
    } else // horizontal edge
    {
      // projecting game coordinate to led coordinate
      uint16_t edge_base_x = 1 + (((x - 1) / 2) * (inner_box_size + 1));
      uint16_t edge_base_y = ((y / 2) * (inner_box_size + 1));

      // print the edge on the led
      uint16_t edge_offset;
      for (edge_offset = 0; edge_offset < inner_box_size; edge_offset++)
      {
        matrixrgb_write_pixel(edge_base_x + edge_offset,
                                   edge_base_y, color);
      }
      return 0;
    }
    return 0;
  }

  /*
   * write the box onto the led matrix
   * empty box should be off
   * inner box are of size (inner_box_size) x (inner_box_size)
   */
  uint8_t write_led_box(uint16_t x, uint16_t y, uint16_t status)
  {
    // projecting game x,y into the led matrix coordinates
    uint16_t box_base_x = 1 + (((x - 1) / 2) * (inner_box_size + 1));
    uint16_t box_base_y = 1 + (((y - 1) / 2) * (inner_box_size + 1));

    // determining color
    uint8_t color = status + 5;

    // unselected box should have no color
    if (status == 0)
    {
      color = BLACK;
    }

    // writing the actual pixels
    uint16_t x_offset, y_offset;
    for (x_offset = 0; x_offset < inner_box_size; x_offset++)
    {
      for (y_offset = 0; y_offset < inner_box_size; y_offset++)
      {
        // calculate led coordinates
        uint16_t led_y = y_offset + box_base_y;
        uint16_t led_x = x_offset + box_base_x;
        matrixrgb_write_pixel(led_x, led_y,
                                   color);
      }
    }
    return 0;s
 }

/*
  * write the dot onto the led matrix
  * this is quite simple as dot is just one led
*/
uint8_t write_led_dot(uint16_t x, uint16_t y, uint16_t status)
{
    // projecting game x,y into the led matrix coordinates
    uint16_t led_dot_x = (x * (box_size - 1) / 2);
    uint16_t led_dot_y = (y * (box_size - 1) / 2);

    // determining color
    uint16_t color = status + 1;

    // writing the actual pixels
    matrixrgb_write_pixel(led_dot_x, led_dot_y, color);
    return 0;
 }

uint8_t write_player_score(struct Player player)
{
    //write player number
    if(player.ID == 1){//p1
        write_digit(4, 1, BLUE);
    }
    else{//p2
        write_digit(4, 2, BLUE);
    }

    //write 10th digit score
    if(player.score < 10){//write zero for score less than 10
        write_digit(10, 0, BLUE);
    }else //write tenth digit
    {
        write_digit(10, player.score /10, BLUE);
    }

    write_digit(14, player.score % 10, BLUE);

    return 0;
}


/*
 * write the given digit at the starting y and the given color
 * each digit is converted into a 5x3 matrix to be written
 * then its passed into a function that actually writes them to the led matrix
 */
uint8_t write_digit(uint16_t start_y, int digit, int color)
{
      switch(digit){
      case 0:
      {
          uint8_t zero_matrix[5][3] =
              {
               {1, 1, 1},
               {1, 0, 1},
               {1, 0, 1},
               {1, 0, 1},
               {1, 1, 1},
          };
          write_digit_to_led(start_y ,zero_matrix, color);
          break;
      }
      case 1:
      {
          uint8_t one_matrix[5][3] =
          {
           {0, 0, 1},
           {0, 0, 1},
           {0, 0, 1},
           {0, 0, 1},
           {0, 0, 1},
          };
          write_digit_to_led(start_y, one_matrix, color);
          break;
      }
      case 2:
      {
          uint8_t two_matrix[5][3] =
            {
             {1, 1, 1},
             {0, 0, 1},
             {1, 1, 1},
             {1, 0, 0},
             {1, 1, 1},
            };
          write_digit_to_led(start_y, two_matrix, color);

          break;
      }
      case 3:
      {
          uint8_t three_matrix[5][3] =
            {
             {1, 1, 1},
             {0, 0, 1},
             {1, 1, 1},
             {0, 0, 1},
             {1, 1, 1},
            };
          write_digit_to_led(start_y, three_matrix, color);
          break;
      }
      case 4:
      {
          uint8_t four_matrix[5][3] =
            {
             {1, 0, 1},
             {1, 0, 1},
             {1, 1, 1},
             {0, 0, 1},
             {0, 0, 1},
            };
          write_digit_to_led(start_y, four_matrix, 1);
          break;
      }
      case 5:
      {
          uint8_t five_matrix[5][3] =
            {
             {1, 1, 1},
             {1, 0, 0},
             {1, 1, 1},
             {0, 0, 1},
             {1, 1, 1},
            };
          write_digit_to_led(start_y, five_matrix, 1);
          break;
      }
      case 6:
      {
          uint8_t six_matrix[5][3] =
            {
             {1, 1, 1},
             {1, 0, 0},
             {1, 1, 1},
             {1, 0, 1},
             {1, 1, 1},
            };
          write_digit_to_led(start_y, six_matrix, 1);
          break;
      }
      case 7:
      {
          uint8_t seven_matrix[5][3] =
            {
             {1, 1, 1},
             {0, 0, 1},
             {0, 0, 1},
             {0, 0, 1},
             {0, 0, 1},
            };
          write_digit_to_led(start_y, seven_matrix, 1);
          break;
      }
      case 8:
      {
          uint8_t eight_matrix[5][3] =
            {
             {1, 1, 1},
             {1, 0, 1},
             {1, 1, 1},
             {1, 0, 1},
             {1, 1, 1},
            };
          write_digit_to_led(start_y, eight_matrix, 1);
          break;
      }
      case 9:
      {
          uint8_t nine_matrix[5][3] =
            {
             {1, 1, 1},
             {1, 0, 1},
             {1, 1, 1},
             {0, 0, 1},
             {0, 0, 1},
            };
          write_digit_to_led(start_y, nine_matrix, 1);
          break;
      }
      default:
      {
          uint8_t blank_matrix[5][3] =
                {
                 {0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0},
                };
          write_digit_to_led(start_y, blank_matrix, 1);

      }

      }
      return 0;
}

/*
 * write the digit matrix to the led matrix at the given start point
 */

void write_digit_to_led(uint16_t start_y, uint8_t score_matrix[5][3], int color){
    int x, y;
    for (x = 27; x < 32; x++){
        for (y = start_y; y < start_y+3; y++){
            matrixrgb_write_pixel(x, y, score_matrix[x-27][y-start_y] * color);
        }
    }
}

/*
 * write the the word ER then the error code at the bottom right corner
 */
void write_error(int error)
{
    // matrix for the letters ER
    uint8_t error_matrix[5][7] =
          {
           {1, 1, 1, 0, 1, 1, 1},
           {1, 0, 0, 0, 1, 0, 1},
           {1, 1, 1, 0, 1, 1, 1},
           {1, 0, 0, 0, 1, 1, 0},
           {1, 1, 1, 0, 1, 0, 1},
          };
    int row, col;
    for (col = 27; col < 32; col++){
        for (row = 21; row < 28; row++){
            matrixrgb_write_pixel(col, row, (error_matrix[col-27][row-21]) * RED);
        }
    }
    //write the error code
    write_digit(29, error, RED);

}

/*
 * writes the winner of the game at the bottom of the board
 * takes a custom letter matrix then write it to the led matrix
 */
void write_winner(int state)
{
    switch(state)
    {
        case 0://p1 wins
        {
            // "p1 wins" letter matrix
            uint8_t winner_matrix[5][26] =
                  {
                   {1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1},
                   {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0},
                   {1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
                   {1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
                   {1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1},
                  };
            int col, row;
            for (col = 27; col < 32; col++){
                for (row = 3; row < 29; row++){
                    matrixrgb_write_pixel(col, row, (winner_matrix[col-27][row-3])*6);
                }
            }
            break;
        }
        case 1://p2 wins
        {
            //"p2 wins letter matrix"
            uint8_t winner_matrix[5][26] =
                  {
                   {1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1},
                   {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0},
                   {1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
                   {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1},
                   {1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1},
                  };
            int col, row;
            for (col = 27; col < 32; col++){
                for (row = 3; row < 29; row++){
                    matrixrgb_write_pixel(col, row, (winner_matrix[col-27][row-3])*7);
                }
            }
            break;
        }
        case 2://tie
        {
            uint8_t tie_matrix[5][17] =
                  {
                   {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                   {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
                   {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                   {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
                   {0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                  };
            int col, row;
            for (col = 27; col < 32; col++){
                for (row = 1; row < 18; row++){
                    matrixrgb_write_pixel(col, row, (tie_matrix[col-27][row-1]));
                }
            }
            break;
        }
        default:
        {
            clear_bottom();
        }
    }
}

/*
 * clear the bottom of the board where the error muscles are displayed
 * from 21 to end of column
 */
void clear_message()
{
    int row, col;
    for (col = 27; col < 32; col++){
        for (row = 21; row < 32; row++){
            matrixrgb_write_pixel(col, row, BLACK);
        }
    }
}

/*
 * clear the entire bottom where both score and error messages are displayed
 */
void clear_bottom()
{
    int row, col;
    for (col = 27; col < 32; col++){
        for (row = 0; row < 32; row++){
            matrixrgb_write_pixel(col, row, 0);
        }
    }
}

/*
 * all the dots should be lighted to the dot color to blue
*/
void write_initial_dots()
{
    uint16_t dot_x, dot_y;
    //print all the dots there is one more row and column of dots then boxes
    for (dot_x = 0; dot_x < ROWS + 1; dot_x++)
    {
        for (dot_y = 0; dot_y < COLUMNS + 1; dot_y++)
        {
        // projecting game x,y into the led matrix coordinates
        uint16_t led_dot_x = (dot_x * (box_size - 1));
        uint16_t led_dot_y = (dot_y * (box_size - 1));

        //write the pixel as blue
        matrixrgb_write_pixel(led_dot_x, led_dot_y, BLUE);
  }
}
}

// end of file
