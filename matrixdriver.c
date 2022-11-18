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

volatile extern uint8_t g_led_matrix[LED_MATRIX_SIZE][LED_MATRIX_SIZE];
//set all pins to output
void init_matrix(){
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2  | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
    P6 -> OUT &= ~ GPIO_PIN5;//R1, B1
}

//select one out of 16 rows
//
void select_row(uint16_t select){
    //clear all bits
    P1 -> OUT &= ~GPIO_PIN5;//a
    P4 -> OUT &= ~(GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN7 );//b, c, and d

    //set bits
    //set a
    P1 -> OUT |= (select & A_SELECT_MASK) << 5;

    //set b select
    P4 -> OUT |= (select & B_SELECT_MASK) << 1;

    //set c select
    P4 -> OUT |= (select & C_SELECT_MASK) << 5;

    //set d select
    P4 -> OUT |= (select & D_SELECT_MASK) << 1;


    return;
}

//sets rgb 1 and rgb2 data bits to be written
void set_rgb_data(uint8_t rgb1, uint8_t rgb2){
    //reset colors
    P3 -> OUT &= ~(GPIO_PIN2 | GPIO_PIN3) ;//R1, B1
    P4 -> OUT &= ~(GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN3);//R2, B2, C3
    P6 -> OUT &= ~(GPIO_PIN1);//G1

    //set rgb pins
    P3 -> OUT |= (rgb1 & RED_MASK)  | ((rgb1 & BLUE_MASK) << 3);//r1 b1
    P6 -> OUT |= rgb1 & GREEN_MASK;//G1
    P4 -> OUT |= ((rgb2 & RED_MASK) >> 1) | ((rgb2 & GREEN_MASK) >> 1) | ((rgb2 & BLUE_MASK) << 3);// r2, g2, b2

}

//latch on to signify end of a row
void latch_on()
{
    P4 -> OUT |= GPIO_PIN5;
}

//latch off to start a new data row
void latch_off()
{
    P4 -> OUT &= ~ GPIO_PIN5;//R1, B1
}

//toggle the clock
void toggle_clock()
{
    P5 -> OUT |= GPIO_PIN4;
    //Clock_Delay1us(5);
    P5 -> OUT &= ~ GPIO_PIN4;//R1, B1

}

//refreshboard sequence

void refresh_led_board(){
    int i, j;
    for(j = 0; j < 16; j++){
        latch_off();
        //GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
        //select_row(j);
        for(i = 0; i<32; i++){
            set_rgb_data(g_led_matrix[j][i], g_led_matrix[j + 16][i]);
            select_row(j);
            toggle_clock();
            //Clock_Delay1us(1);
        }
        latch_on();

    }

    //Clock_Delay1us(10);

}

void led_board_off(){
    int i, j;
    for(j = 0; j < 16; j++){
        latch_off();
        //GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
        //select_row(j);
        for(i = 0; i<32; i++){
            set_rgb_data(0, 0);
            select_row(j);
            toggle_clock();
            //Clock_Delay1us(1);
        }
        latch_on();

    }

    //Clock_Delay1us(10);
}

