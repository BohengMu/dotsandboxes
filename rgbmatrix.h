/*
 * rgbmatrix.h
 *
 *  Created on: Oct 11, 2022
 *      Author: jhlee
 */

#ifndef RGBMATRIX_H_
#define RGBMATRIX_H_

#include "spi.h"
#include "Clock.h"

#define MATRIXRGB_RETVAL  uint8_t

#define MATRIXRGB_OK           0x00
#define MATRIXRGB_INIT_ERROR   0xFF

//defines for patterns of RGB panels
#define MATRIXRGB_PATTERN_1_MAP_5MM               0x00
#define MATRIXRGB_PATTERN_1_MAP_6MM               0x01
#define MATRIXRGB_PATTERN_2H_MAP_5MM              0x02
#define MATRIXRGB_PATTERN_2H_MAP_6MM              0x03
#define MATRIXRGB_PATTERN_2V_MAP_5MM              0x04
#define MATRIXRGB_PATTERN_2V_MAP_6MM              0x05
#define MATRIXRGB_PATTERN_3H_MAP_5MM              0x06
#define MATRIXRGB_PATTERN_3H_MAP_6MM              0x07
#define MATRIXRGB_PATTERN_3V_MAP_5MM              0x08
#define MATRIXRGB_PATTERN_3V_MAP_6MM              0x09
#define MATRIXRGB_PATTERN_4H_MAP_5MM              0x0A
#define MATRIXRGB_PATTERN_4H_MAP_6MM              0x0B
#define MATRIXRGB_PATTERN_4V_MAP_5MM              0x0C
#define MATRIXRGB_PATTERN_4V_MAP_6MM              0x0D
#define MATRIXRGB_PATTERN_4S_MAP_5MM              0x0E
#define MATRIXRGB_PATTERN_4S_MAP_6MM              0x0F

// struct for matrixrgb pixel
typedef struct
{
    uint16_t pixel_width;
    uint16_t pixel_height;
    uint16_t ram_size;

} matrixrgb_pixel_t;


// struct for coordinates of pixel
typedef struct
{
    uint16_t x_cord;
    uint16_t y_cord;

} matrixrgb_cord_t;



typedef struct
{

    /**
    // Output pins

    digital_out_t rst;
    digital_out_t slp;
    digital_out_t cs;

    // Input pins

    digital_in_t rdy;
    */

    // Modules

    //spi_master_t spi;
    matrixrgb_pixel_t device_pixel;
    matrixrgb_cord_t device_font_cord;
    uint16_t device_pattern_delay;
    //pin_name_t chip_select;

} matrixrgb_t;

// matrix rgb configuration
typedef struct
{
    // Communication gpio pins
    /**
    pin_name_t miso;
    pin_name_t mosi;
    pin_name_t sck;
    pin_name_t cs;

    // Additional gpio pins

    pin_name_t rst;
    pin_name_t slp;
    pin_name_t rdy;
*/
    // static variable

    uint32_t spi_speed;
    uint8_t spi_mode;
    uint8_t cs_polarity;

} matrixrgb_cfg_t;

/**
 * @brief Config Object Initialization function.
 *
 * @param cfg  Click configuration structure.
 *
 * @description This function initializes click configuration structure to init state.
 * @note All used pins will be set to unconnected state.
 */
void matrixrgb_cfg_setup ( matrixrgb_cfg_t *cfg );

/**
 * @brief Initialization function.
 * @param matrixrgb Click object.
 * @param cfg Click configuration structure.
 *
 * @description This function initializes all necessary pins and peripherals used for this click.
 */
MATRIXRGB_RETVAL matrixrgb_init ( matrixrgb_t *ctx, matrixrgb_cfg_t *cfg );

/**
 * @brief Generic transfer function.
 *
 * @param ctx          Click object.
 * @param wr_buf       Write data buffer
 * @param wr_len       Number of byte in write data buffer
 * @param rd_buf       Read data buffer
 * @param rd_len       Number of byte in read data buffer
 *
 * @description Generic SPI transfer, for sending and receiving packages
 */
void matrixrgb_generic_transfer
(
    matrixrgb_t *ctx,
    uint8_t *wr_buf,
    uint16_t wr_len,
    uint8_t *rd_buf,
    uint16_t rd_len
);


/**
 * @brief Hard reset of the Matrix RGB click
 *
 * @param ctx          Click object.
 *
 * @description Function takes 300 ms.
 */
void matrixrgb_device_reset ( matrixrgb_t *ctx );

/**
 * @brief Firmware Initialization
 *
 * @param ctx          Click object.
 * @param pattern_id   Panel Pattern ID - more info inside firmware documentation
 *
 * @description This function mus be executed right after driver initialization.
 */
uint8_t matrixrgb_device_settings ( matrixrgb_t *ctx, uint8_t pattern_id );

/**
 * @brief Set Power
 *
 * @param ctx           Click object.
 * @param power_state   Power State (0 - OFF / 1 - ON)
 *
 */
void matrixrgb_set_power ( matrixrgb_t *ctx, uint8_t power_state );

/**
 * @brief Set Brightness
 *
 * @param ctx           Click object.
 * @param brightness    Brightness intensity
 *
 * @warning
 * In case of high brightness level flickering may appears due to fact
 * that this is software brightness implementation.
 */
void matrixrgb_set_brightness ( matrixrgb_t *ctx, uint8_t brightness );

/**
 * @brief Write Pixel
 *
 * @param ctx           Click object.
 * @param x             horizontal position
 * @param y             vertical position
 * @param color         pixel color
 *
 * @return
 * 0 OK / 1 Error
 *
 * @note
 * Error may appear in case of wrong X or Y positions.
 */
uint8_t matrixrgb_write_pixel ( matrixrgb_t *ctx, uint16_t x, uint16_t y, uint16_t color );

/**
 * @brief Fill Screen
 *
 * @param ctx           Click object.
 * @param color         color screen color
 *
 */
void matrixrgb_fill_screen ( matrixrgb_t *ctx, uint16_t color );



#endif /* RGBMATRIX_H_ */
