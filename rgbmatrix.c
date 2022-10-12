/*
 * rgbmatrix.c
 *
 *  Created on: Oct 11, 2022
 *      Author: jhlee
 */

#include "rgbmatrix.h"
#include "spi.h"
#include "Clock.h"

#define MATRIXRGB_DUMMY                                     0

#define MAX_ram_sizeIZE                                  4096

#define MATRIXRGB_CMD_POWER                              0x01
#define MATRIXRGB_CMD_BRIGHTNESS                         0x02
#define MATRIXRGB_CMD_LOAD_IMG                           0x03
#define MATRIXRGB_CMD_LOAD_PIX                           0x04
#define MATRIXRGB_CMD_PAN_SETUP                          0x10

static void send_command ( matrixrgb_t *ctx,  uint8_t cmd, uint8_t arg );


/*
 * set up the matrix with spi speed/mode
 */
void matrixrgb_cfg_setup ( matrixrgb_cfg_t *cfg )
{
    // Communication gpio pins
    /**
    cfg->sck = HAL_PIN_NC ;
    cfg->miso = HAL_PIN_NC;
    cfg->mosi = HAL_PIN_NC;
    cfg->cs = HAL_PIN_NC;

    // Additional gpio pins

    cfg->rst = HAL_PIN_NC;
    cfg->slp = HAL_PIN_NC;
    cfg->rdy = HAL_PIN_NC;
    */
    cfg->spi_speed = 100000;
    cfg->spi_mode = 01;
    cfg->cs_polarity = EUSCI_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
}


/*
 * initialize other spi configs and clock configs
 */
MATRIXRGB_RETVAL matrixrgb_init ( matrixrgb_t *ctx, matrixrgb_cfg_t *cfg )
{

    //initialize SPI on MSP432
    eUSCI_SPI_MasterConfig spi_cfg;
    spi_cfg.clockSourceFrequency= cfg->spi_speed;
    spi_cfg.clockPhase = EUSCI_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_cfg.clockPolarity = EUSCI_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    spi_cfg.desiredSpiClock = cfg->spi_speed;
    spi_cfg.spiMode = EUSCI_SPI_4PIN_UCxSTE_ACTIVE_HIGH;
    spi_cfg.msbFirst = EUSCI_SPI_MSB_FIRST;
    spi_cfg.selectClockSource = EUSCI_SPI_CLOCKSOURCE_ACLK;

    SPI_initMaster(EUSCI_A0_BASE, &spi_cfg);
    // set Output pins

    //digital_out_init( &ctx->rst, cfg->rst );
    //digital_out_init( &ctx->slp, cfg->slp );

    // set Input pins

    //digital_in_init( &ctx->rdy, cfg->rdy );

    return MATRIXRGB_OK;
}

void matrixrgb_generic_transfer
(
    matrixrgb_t *ctx,
    uint8_t *wr_buf,
    uint16_t wr_len,
    uint8_t *rd_buf,
    uint16_t rd_len
)
{
    SPI_transmitData(EUSCI_A0_BASE, wr_buf);
    rd_buf = SPI_receiveData(EUSCI_A0_BASE);

}

//commenting out the resent
#if 0
void matrixrgb_device_reset ( matrixrgb_t *ctx )
{
    digital_out_high( &ctx->rst );
    Delay_100ms();
    digital_out_low( &ctx->rst );
    Delay_100ms();
    digital_out_high( &ctx->rst );
    Delay_100ms();
}
#endif

uint8_t matrixrgb_device_settings ( matrixrgb_t *ctx, uint8_t pattern_id )
{
    uint8_t tmp;
    switch  ( pattern_id )
    {
        case 0: case 1:
        {
            ctx->device_pixel.pixel_width = 32;
            ctx->device_pixel.pixel_height = 32;
            ctx->device_pixel.ram_size = 1024;
            break;
        }
        case 2: case 3:
        {
            ctx->device_pixel.pixel_width = 64;
            ctx->device_pixel.pixel_height = 32;
            ctx->device_pixel.ram_size = 2048;
            break;
        }
        case 4: case 5:
        {
            ctx->device_pixel.pixel_width = 32;
            ctx->device_pixel.pixel_height = 64;
            ctx->device_pixel.ram_size = 2048;
            break;
        }
        case 6: case 7:
        {
            ctx->device_pixel.pixel_width = 96;
            ctx->device_pixel.pixel_height = 32;
            ctx->device_pixel.ram_size = 3072;
            break;
        }
        case 8: case 9:
        {
            ctx->device_pixel.pixel_width = 32;
            ctx->device_pixel.pixel_height = 96;
            ctx->device_pixel.ram_size = 3072;
            break;
        }
        case 10: case 11:
        {
            ctx->device_pixel.pixel_width = 128;
            ctx->device_pixel.pixel_height = 32;
            ctx->device_pixel.ram_size = 4096;
            break;
        }
        case 12: case 13:
        {
            ctx->device_pixel.pixel_width = 32;
            ctx->device_pixel.pixel_height = 128;
            ctx->device_pixel.ram_size = 4096;
            break;
        }
        case 14: case 15:
        {
            ctx->device_pixel.pixel_width = 64;
            ctx->device_pixel.pixel_height = 64;
            ctx->device_pixel.ram_size = 4096;
            break;
        }
        default :
        {
            return 1;
        }
    }
    tmp = pattern_id;
    send_command( ctx, MATRIXRGB_CMD_PAN_SETUP, tmp );

    return 0;
}

void matrixrgb_set_power ( matrixrgb_t *ctx, uint8_t power_state )
{
    uint8_t tmp;
    tmp = power_state;
    send_command( ctx, MATRIXRGB_CMD_POWER, tmp  );
}


/*
 * set the brightness of the screen
 */
void matrixrgb_set_brightness ( matrixrgb_t *ctx, uint8_t brightness )
{
    uint8_t tmp;
    tmp = brightness;
    send_command( ctx, MATRIXRGB_CMD_BRIGHTNESS, tmp );
}


/*
 * write a pixel at coordinates x,y to a color
 */
uint8_t matrixrgb_write_pixel ( matrixrgb_t *ctx, uint16_t x, uint16_t y, uint16_t color )
{
    uint8_t cmd;
    uint16_t pos;
    uint8_t tmp[ 4 ];

    if ( ( x >= ctx->device_pixel.pixel_width ) || ( y >= ctx->device_pixel.pixel_height ) )
    {
        return 1;
    }

    cmd = MATRIXRGB_CMD_LOAD_PIX;
    pos = ( ( y * ctx->device_pixel.pixel_width ) + x );

    tmp[0] = color;
    tmp[1] = color >> 8;
    tmp[2] = pos;
    tmp[3] = pos >> 8;


    SPI_transmitData(EUSCI_A0_BASE, &cmd);
    SPI_transmitData(EUSCI_A0_BASE, tmp[0]);
    SPI_transmitData(EUSCI_A0_BASE, tmp[1]);
    SPI_transmitData(EUSCI_A0_BASE, tmp[2]);
    SPI_transmitData(EUSCI_A0_BASE, tmp[3]);
    Clock_Delay1ms(1);

    return 0;
}

/*
 * fill screen with a specific color
 */
void matrixrgb_fill_screen ( matrixrgb_t *ctx, uint16_t color )
{
    uint8_t cmd;
    uint16_t pos;
    uint8_t tmp[ 2 ];

    cmd = MATRIXRGB_CMD_LOAD_IMG;
    tmp[ 0 ] = color;
    tmp[ 1 ] = color >> 8;


    SPI_transmitData(EUSCI_A0_BASE, &cmd);

    for ( pos = 0; pos < ctx->device_pixel.ram_size; pos++ )
    {

        SPI_transmitData(EUSCI_A0_BASE, tmp[0]);
        SPI_transmitData(EUSCI_A0_BASE, tmp[1]);

        Clock_Delay1ms(1);
    }

    Clock_Delay1ms(1);
}

/*
 * initialize matrix
 */
void LEDmatrixrgb_init ( void )
{
    matrixrgb_cfg_t cfg;
    static matrixrgb_t matrixrgb;

    //  Click initialization.

    matrixrgb_cfg_setup( &cfg );
    matrixrgb_init( &matrixrgb, &cfg );

   // matrixrgb_device_reset( &matrixrgb );

    //matrixrgb_pattern_settings( &matrixrgb, MATRIXRGB_PATTERN_1_MAP_5MM, 1000 );
    matrixrgb_set_power( &matrixrgb, 1 );
    Clock_Delay1ms( 1000 );

    matrixrgb_fill_screen( &matrixrgb, 0xFFFF );
    Clock_Delay1ms( 1000 );
}


/*
 * testing matrix rgb
 */
void LEDmatrixrgb_task ( )
{
    uint16_t test;
    static matrixrgb_t matrixrgb;

    // Brightness Test
    for ( test = 5; test < 50; test++ )
    {
        matrixrgb_set_brightness( &matrixrgb, test );
        Clock_Delay1ms( 50 );
    }

    for ( test = 50; test > 5; test-- )
    {
        matrixrgb_set_brightness( &matrixrgb, test );
        Clock_Delay1ms( 50 );
    }

    // Pixel Write Test
    matrixrgb_fill_screen( &matrixrgb, 0x0000 );
    for ( test = 0; test < 32; test++ )
    {
        matrixrgb_write_pixel( &matrixrgb, test, test, 0xF100 );
        Clock_Delay1ms( 100 );
    }
    for ( test = 32; test > 0; test-- )
    {
        matrixrgb_write_pixel( &matrixrgb, 31 - test, test, 0xF100 );
        Clock_Delay1ms( 100 );
    }
    Clock_Delay1ms( 2000 );

}

//private function send_command
static void send_command ( matrixrgb_t *ctx, uint8_t cmd, uint8_t arg )
{
    uint8_t tx_buf[ 2 ];

    //wait_int_pin( ctx );

    tx_buf[ 0 ] = cmd;
    tx_buf[ 1 ] = arg;

    /**
    spi_master_select_device( ctx->chip_select );
    spi_master_write( &ctx->spi, tx_buf, 2 );
    spi_master_deselect_device( ctx->chip_select );
    Delay_1ms();
    */

    SPI_transmitData(EUSCI_A0_BASE, tx_buf[0]);
    SPI_transmitData(EUSCI_A0_BASE, tx_buf[1]);
    Clock_Delay1ms(1);
}

//end of file
