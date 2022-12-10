/*
 * Scoreboard.h
 *
 *  Created on: Nov 18, 2022
 *      Author: James Tsai
 */
#include <stdint.h>
#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

void configure_scoreboard(void);
void writeMAX6955(uint32_t driver, uint8_t command, uint8_t data);
void initMAX6955(uint32_t driver);
void writeP1(uint32_t driver);
void clear(uint32_t driver);

#endif /* SCOREBOARD_H_ */
