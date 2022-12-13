/*
 * matrixdriver.c
 *
 *  Created on: Nov 13, 2022
 *      Author: mub91
 */


#include "matrixdriver.h"
#include "clock.h"
#include "stdint.h"
#include "msp.h"
#include "gpio.h"
#include "board.h"
#include "defines.h"

volatile extern uint8_t g_led_matrix[LED_MATRIX_SIZE][LED_MATRIX_SIZE];
//set all pins to output and low on 5.5 and 6.5
/*
 *      6.0 pin 3 driver left addr 0
        R1 3.2                  G1 6.1
        B1 3.3                  G2 4.0
        R2 4.1                  B 4.2
        B2 4.3                  D 4.4
        A 1.5                   LAT 4.5
        4.6 pin 6 driver left addr 2        C 4.7
        6.5 pin 5 driver left clk    *      ClK 4.4
        6.4 pin 4 driver left data      OE 5.5
 *
 */
void init_matrix(){
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2  | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5);
}

//select one out of 16 rows
void select_row(uint16_t select){
    //clear all bits
    P1 -> OUT &= ~GPIO_PIN5;//a
    P4 -> OUT &= ~(GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN7 );//b, c, and d

    // A P1.5
    // B P4.2
    // C P4.7
    // D P4.4

    // select is ODD: set A pin (GPIO 1.5) high
    if (select & 0x01) {
        SELECT_A;
    }

    // sets DCB of select bits based on select
    switch(select) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:
        SELECT_B
        break;
    case 4:
    case 5:
        SELECT_C;
        break;
    case 6:
    case 7:
        SELECT_C;
        SELECT_B;
        break;
    case 8:
    case 9:
        SELECT_D;
        break;
    case 10:
    case 11:
        SELECT_D;
        SELECT_B;
        break;
    case 12:
    case 13:
        SELECT_D;
        SELECT_C;
        break;
    case 14:
    case 15:
        SELECT_D;
        SELECT_C;
        SELECT_B;
        break;

    }

    return;
}

//sets rgb 1 and rgb2 data bits to be written
void set_rgb_data(uint8_t rgb1, uint8_t rgb2){
    //reset colors
    P3 -> OUT &= ~(GPIO_PIN2 | GPIO_PIN3) ;//R1, B1
    P4 -> OUT &= ~(GPIO_PIN1 | GPIO_PIN0 | GPIO_PIN3);//R2, B2, G2
    P6 -> OUT &= ~(GPIO_PIN1);//G1

    // R1 P3.2
    // G1 P6.1
    // B1 P3.3

    // rgb1 is ODD: set B1 pin (GPIO 3.3) high
    if (rgb1 & 0x01) {
        P3 -> OUT |= GPIO_PIN3;
    }

    switch(rgb1) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:     // set G1 (P6.1) high in case 2 and 3
        P6 -> OUT |= GPIO_PIN1;
        break;
    case 4:
    case 5:     // set R1 (P3.2) high in case 4 and 5
        P3 -> OUT |= GPIO_PIN2;
        break;
    case 6:
    case 7:     // set R1 (P3.2) and G1 (P6.1) high in case 6 and 7
        P6 -> OUT |= GPIO_PIN1;
        P3 -> OUT |= GPIO_PIN2;
        break;
    }

    // R2 P4.1
    // G2 P4.0
    // B2 P4.3

    // rgb2 is ODD: set B2 pin (GPIO 4.3) high
    if (rgb2 & 0x01) {
        P4 -> OUT |= GPIO_PIN3;
    }

    switch(rgb2) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:     // set G2 (P4.0) high in case 2 and 3
        P4 -> OUT |= GPIO_PIN0;
        break;
    case 4:
    case 5:     // set R2 (P4.1) high in case 4 and 5
        P4 -> OUT |= GPIO_PIN1;
        break;
    case 6:
    case 7:     // set R1 (P4.1) and G1 (P4.0) high in case 6 and 7
        P4 -> OUT |= GPIO_PIN0;
        P4 -> OUT |= GPIO_PIN1;
        break;
    }

}

/* Refresh board sequence
 * LED Matrix runs off 16 bit multiplexing, so it writes two 16x32 pixel boards
 * at a time, which is defined by first_row and second_row.
 * g_led_matrix stores all the pixel data to write to the board
 */
void refresh_led_board(){
    int i, j;
    for(j = 0; j < 16; j++){
        //define first and second rows to write to
        select_row(j);
        uint8_t* first_row = g_led_matrix[j];
        uint8_t* second_row = g_led_matrix[j + 16];
        P4 -> OUT &= ~ GPIO_PIN5;//latch off to start new data row
            for(i = 0; i<32; i++){
                set_rgb_data(g_led_matrix[j][i], second_row[i]);
                //toggle clock
                P5 -> OUT |= GPIO_PIN4;
                P5 -> OUT &= ~ GPIO_PIN4;//R1, B1
            }
        P4 -> OUT |= GPIO_PIN5;//latch off
            P4 -> OUT &= ~ GPIO_PIN5;//R1, B1
            //turn off the board by setting all pixels to black to prevent aliasing
            for(i = 0; i<32; i++){
                set_rgb_data(0, 0);
                //toggle clock
                P5 -> OUT |= GPIO_PIN4;
                P5 -> OUT &= ~ GPIO_PIN4;//R1, B1
            }
        P4 -> OUT |= GPIO_PIN5;//latch on to signify end of row

    }

}
//end of file
