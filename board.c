

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

  int box_size;
  int inner_box_size;

  //led board
  volatile uint8_t g_led_matrix[LED_MATRIX_SIZE][LED_MATRIX_SIZE];

  //game state prin representations
  char verticle_edges[4] = {' ', '-', '^', 'x'};
  char horizontal_edges[4] = {' ', '|', '>', 'x'};
  char dots[2] = {'.', 'o'};
  char boxes[3] = {' ', '1', '2'};

  /*
   * Reconfigure the board to all zeros
   */
  void init_board()
  {
    int row_size = ((LED_MATRIX_ROWS-1)/ROWS +1);
    int column_size = ((LED_MATRIX_COLUMNS-1)/COLUMNS +1);
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

    //set game state to zeros
    int i, j;
    for (i = 0; i < 2 * ROWS + 1; i++) {
      for (j = 0; j < 2 * COLUMNS + 1; j++) {
        g_board[i][j] = 0;
      }
    }


    for(i = 0; i < ROWS + 1; i ++){
        for(j = 0;j< COLUMNS + 1; j ++){
            write_led_dot(i*2, j*2, 0);
        }
    }
    uint8_t p_matrix[5][3] =
          {
           {1, 1, 0},
           {1, 0, 1},
           {1, 1, 0},
           {1, 0, 0},
           {1, 0, 0},
          };

          uint8_t color = 6;
          uint16_t x, y;
          for (x = 27; x < 32; x++){
              for (y = 0; y < 3; y++){
                  matrixrgb_write_pixel(x, y, p_matrix[x-27][y]);
              }
          }
  }

  /*
   * Print the current game state board and the led board
   * TODO: delete this in the final version as a terminal will not be needed
   */
  void print_board()
  {
    //printing game state
    printf("--------------gamestate of size %i x %i--------------\n", ROWS, COLUMNS);
    int row, column;
    for (row = 0; row < 2 * ROWS; row += 2)
    {
      //print two rows at a time
      char dotrow[2 * COLUMNS + 2];
      char boxrow[2 * COLUMNS + 2];

      //add terminal character
      dotrow[2 * COLUMNS + 1] = '\0';
      boxrow[2 * COLUMNS + 1] = '\0';

      //prepare the two rows
      for (column = 0; column < 2 * COLUMNS + 1; column++)
      {
        //convert dots row to characters
        dotrow[column] = (column % 2 == 0) ? dots[g_board[row][column]]
                                           : verticle_edges[g_board[row][column]];
        //convert boxes row to characters
        boxrow[column] = (column % 2 == 0)
                             ? horizontal_edges[g_board[row + 1][column]]
                             : boxes[g_board[row + 1][column]];
      }

      //print the two rows
      printf("%s\n%s\n", dotrow, boxrow);
    }

    //set up for the last row
    char dotrow[2 * COLUMNS + 2];
    dotrow[2 * COLUMNS + 1] = '\0';

    // last row
    for (column = 0; column < 2 * COLUMNS + 1; column++)
    {
      dotrow[column] = (column % 2 == 0)
                           ? dots[g_board[2 * ROWS][column]]
                           : verticle_edges[g_board[2 * ROWS][column]];
    }
    printf("%s\n", dotrow);

    //printing led representation
  //  printf("--------------led of size %i x %i--------------\n", LED_MATRIX_SIZE, LED_MATRIX_SIZE);
  //  int led_row, led_column;
  //  for(led_row = 0; led_row < LED_MATRIX_SIZE; led_row++)
  //  {
  //    char row[LED_MATRIX_SIZE+1];
  //
  //    //add terminal character
  //    row[LED_MATRIX_SIZE] = '\0';
  //
  //    // prepare the row
  //    for (led_column = 0; led_column < LED_MATRIX_SIZE; led_column++){
  //
  //      row[led_column] = 48 + led_matrix[led_row][led_column];
  //    }
  //    printf("%s\n", row);
  //  }
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
      color = 0;
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
      color = 0;
    }

    // writing the actual pixels
    uint16_t x_offset, y_offset;
    for (x_offset = 0; x_offset < inner_box_size; x_offset++)
    {
      for (y_offset = 0; y_offset < inner_box_size; y_offset++)
      {
        // calculate led coords
        uint16_t led_y = y_offset + box_base_y;
        uint16_t led_x = x_offset + box_base_x;
        matrixrgb_write_pixel(led_x, led_y,
                                   color);
      }
    }
    return 0;
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
      if(player.ID == 0){
          write_digit(0,0,4);
      }
      return 0;
  }

  uint8_t write_digit(uint16_t start_x,uint16_t start_y, int digit)
  {
      return 0;
  }


  /*
   * all the dots should be lighted to the dot color 1
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

        //color hard set to 1
        matrixrgb_write_pixel(led_dot_x, led_dot_y, 1);
      }
    }
  }

  // end of file
