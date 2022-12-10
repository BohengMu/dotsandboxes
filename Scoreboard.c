/*
 * Scoreboard.c
 *
 *  Created on: Nov 18, 2022
 *      Author: James Tsai
 */

#include "Scoreboard.h"
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "Scoreboard.h"
#include "Clock.h"
#include "i2c.h"
/*
 * The master input port/bits for the four 7-seg displays are on
 * SCLK1: P6.5
 * SCLK2: P1.7
 * SDA1: P6.4
 * SDA2: P1.6
 *
 */
/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
EUSCI_B_I2C_CLOCKSOURCE_SMCLK, // SMCLK Clock Source
3000000, // SMCLK = 3MHz
EUSCI_B_I2C_SET_DATA_RATE_400KBPS, // Desired I2C Clock of 400khz
0, // No byte counter threshold
EUSCI_B_I2C_NO_AUTO_STOP // No Autostop
};

void configure_scoreboard(void)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
//
//    P6->DIR &= ~(BIT4 | BIT5);
//    P1->DIR &= ~(BIT6 | BIT7);
//    P6->REN |= BIT4;
//    P1->REN |= BIT6;
    // Initializing I2C Master to SMCLK at 400kbs with no autostop
    I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
    I2C_setSlaveAddress(EUSCI_B0_BASE, MAX6955_1_BaseAddress);
    I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
    I2C_enableModule(EUSCI_B0_BASE);
    I2C_clearInterruptFlag(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    I2C_clearInterruptFlag(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    I2C_clearInterruptFlag(EUSCI_B0_BASE, EUSCI_B_I2C_NAK_INTERRUPT);

    Interrupt_enableInterrupt(INT_EUSCIB0);

}


void writeMAX6955(uint32_t driver, uint8_t command, uint8_t data)
{
   I2C_setMode(driver, EUSCI_B_I2C_TRANSMIT_MODE);
   I2C_masterSendMultiByteStart(driver,command);
   I2C_masterSendMultiByteFinish(driver, data);
}

void initMAX6955(uint32_t driver)
{
   writeMAX6955(driver, reg_decodeMode, 0xFF);
   writeMAX6955(driver, reg_scanLimit, 0x07);
   writeMAX6955(driver, reg_configuration, 0x01);
   writeMAX6955(driver, reg_globalIntensity, 0x0F);
   writeMAX6955(driver, reg_digitType, 0x00);
   writeMAX6955(driver, reg_displayTest, 0x01);
   Clock_Delay1ms(1000);
   writeMAX6955(driver, reg_displayTest, 0x00);
   writeMAX6955(driver, 0x04, 0x01);
}
void writeP1(uint32_t driver)
{
    writeMAX6955(driver, reg_digit0, 0x01);
    writeMAX6955(driver, reg_digit1, 0x01);
    writeMAX6955(driver, reg_digit4, 0x01);
    writeMAX6955(driver, reg_digit5, 0x01);
    writeMAX6955(driver, reg_digit6, 0x01);
    writeMAX6955(driver, reg_digit1a, 0x01);
    writeMAX6955(driver, reg_digit2a, 0x01);
}
void clear(uint32_t driver)
{
    writeMAX6955(driver, reg_digit0, 0x00);
    writeMAX6955(driver, reg_digit1, 0x00);
    writeMAX6955(driver, reg_digit2, 0x00);
    writeMAX6955(driver, reg_digit3, 0x00);
    writeMAX6955(driver, reg_digit4, 0x00);
    writeMAX6955(driver, reg_digit5, 0x00);
    writeMAX6955(driver, reg_digit6, 0x00);
    writeMAX6955(driver, reg_digit7, 0x00);
    writeMAX6955(driver, reg_digit0a, 0x00);
    writeMAX6955(driver, reg_digit1a, 0x00);
    writeMAX6955(driver, reg_digit2a, 0x00);
    writeMAX6955(driver, reg_digit3a, 0x00);
    writeMAX6955(driver, reg_digit4a, 0x00);
    writeMAX6955(driver, reg_digit5a, 0x00);
    writeMAX6955(driver, reg_digit6a, 0x00);
    writeMAX6955(driver, reg_digit7a, 0x00);
}
//end of file

