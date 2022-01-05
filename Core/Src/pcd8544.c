/*
 * pcd8544.c
 *
 *  Created on: Jan 2, 2022
 *      Author: cjw
 */

#include "main.h"

/**
 * @brief Initialize the PCD8544: 48 × 84 pixels matrix LCD Set bias and contrast,
 *        enter normal mode.
 *
 * @param  GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4
 *         family
 * @param  GPIO_Pin specifies the port bit to be written.
 *         This parameter can be any combination of GPIO_Pin_x where x can be
 * (0..15).
 * @retval None
 */
void LCD_Init(SPI_HandleTypeDef *hspi)
{
	// SPI must already be initialized
	assert_param(HAL_SPI_GetState(hspi) == HAL_SPI_STATE_READY);

	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET); // set reset pin low
	HAL_Delay(1);                                        // delay
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);   // set high

	// normal mode
	Command(hspi, PCD8544_FUNCTIONSET);

	// Set display to Normal
	Command(hspi, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

/**
 * @brief Send a general SPI command to the LCD display
 * @param command is the single-byte SPI command
 * @param dc_pin is the pin conected to Data/Command pin of the PCD8544
 * @retval None
 */
void Command(SPI_HandleTypeDef *hspi, uint8_t command) {
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &command, 1, 0);
}
