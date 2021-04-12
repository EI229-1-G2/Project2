/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    board.h
 * @brief   Board initialization header file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

/**
 * @brief	The board name 
 */
#define BOARD_NAME "board"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief	Macros for board specific interfaces
 */
/* I2C */
#define I2C_EEPROM_CLK_SRC I2C0_CLK_SRC
#define I2C_EEPROM_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_EEPROM_BASEADDR I2C0
#define I2C_EEPROM_SLAVE_ADDR_7BIT 0x50U
#define I2C_EEPROM_BAUDRATE 100000U
#define I2C_EEPROM_DATA_LENGTH 64U

#define I2C_CAT9555_CLK_SRC I2C3_CLK_SRC
#define I2C_CAT9555_CLK_FREQ CLOCK_GetFreq(I2C3_CLK_SRC)
#define I2C_CAT9555_BASEADDR I2C3
#define I2C_CAT9555_SLAVE_ADDR_7BIT 0x22U
#define I2C_CAT9555_BAUDRATE 200000U
#define I2C_CAT9555_DATA_LENGTH 2U

/* GPIO operation shortcuts */
#define LED1_ON() GPIO_PortClear(BOARD_INITPINS_LED1_GPIO, BOARD_INITPINS_LED1_GPIO_PIN_MASK)
#define LED1_OFF() GPIO_PortSet(BOARD_INITPINS_LED1_GPIO, BOARD_INITPINS_LED1_GPIO_PIN_MASK)
#define LED1_TOGGLE() GPIO_PortToggle(BOARD_INITPINS_LED1_GPIO, BOARD_INITPINS_LED1_GPIO_PIN_MASK)
#define LED2_ON() GPIO_PortClear(BOARD_INITPINS_LED2_GPIO, BOARD_INITPINS_LED2_GPIO_PIN_MASK)
#define LED2_OFF() GPIO_PortSet(BOARD_INITPINS_LED2_GPIO, BOARD_INITPINS_LED2_GPIO_PIN_MASK)
#define LED2_TOGGLE() GPIO_PortToggle(BOARD_INITPINS_LED2_GPIO, BOARD_INITPINS_LED2_GPIO_PIN_MASK)
#define DSPdp_ON() GPIO_PortClear(BOARD_INITPINS_DSPdp_GPIO, BOARD_INITPINS_DSPdp_GPIO_PIN_MASK)
#define DSPdp_OFF() GPIO_PortSet(BOARD_INITPINS_DSPdp_GPIO, BOARD_INITPINS_DSPdp_GPIO_PIN_MASK)
#define DSPdp_TOGGLE() GPIO_PortToggle(BOARD_INITPINS_DSPdp_GPIO, BOARD_INITPINS_DSPdp_GPIO_PIN_MASK)
#define BEEP_ON() GPIO_PortSet(BOARD_INITPINS_BEEP_GPIO, BOARD_INITPINS_BEEP_GPIO_PIN_MASK)
#define BEEP_OFF() GPIO_PortClear(BOARD_INITPINS_BEEP_GPIO, BOARD_INITPINS_BEEP_GPIO_PIN_MASK)

#define KEY1() GPIO_PinRead(BOARD_INITPINS_KEY1_GPIO, BOARD_INITPINS_KEY1_PIN)
#define KEY2() GPIO_PinRead(BOARD_INITPINS_KEY2_GPIO, BOARD_INITPINS_KEY2_PIN)
#define QESA() GPIO_PinRead(BOARD_INITPINS_QESa_GPIO, BOARD_INITPINS_QESa_PIN)
#define QESB() GPIO_PinRead(BOARD_INITPINS_QESb_GPIO, BOARD_INITPINS_QESb_PIN)
#define QESP() GPIO_PinRead(BOARD_INITPINS_QESp_GPIO, BOARD_INITPINS_QESp_PIN)
#define SDCD() GPIO_PinRead(BOARD_INITPINS_SDCD_GPIO, BOARD_INITPINS_SDCD_PIN)
#define SDWP() GPIO_PinRead(BOARD_INITPINS_SDWP_GPIO, BOARD_INITPINS_SDWP_PIN)
#define SW1() GPIO_PinRead(BOARD_INITPINS_SW1_GPIO, BOARD_INITPINS_SW1_PIN)
#define SW2() GPIO_PinRead(BOARD_INITPINS_SW2_GPIO, BOARD_INITPINS_SW2_PIN)
#define SW3() GPIO_PinRead(BOARD_INITPINS_SW3_GPIO, BOARD_INITPINS_SW3_PIN)
#define SW4() GPIO_PinRead(BOARD_INITPINS_SW4_GPIO, BOARD_INITPINS_SW4_PIN)

// PIT0 provides app ticks (uint32) at an interval of 5ms
#define APPTICK_PIT_HANDLER PIT0_IRQHandler
#define APPTICK_PIT_IRQn PIT0_IRQn
#define APPTICK_PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

// Rotary Encoder Switch IRQ
#define BOARD_RESa_IRQ PORTB_IRQn
//#define BOARD_RESa_IRQ_HANDLER PORTB_IRQHandler

/**
 * @brief 	Initialize board specific settings.
 */
void BOARD_InitDebugConsole(void);
void delay(void);
void delay_short(void);
void delay_long(void);
void delay1ms(uint16_t delayTime);
uint8_t InverseByteBits(uint8_t data);
void BOARD_InitI2C(void);
void BOARD_I2C_GPIO(uint16_t ctrlValue);
void BOARD_InitRTC(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */


