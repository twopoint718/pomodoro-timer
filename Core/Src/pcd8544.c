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
 * @param  PinState specifies the value to be written to the selected bit.
 *         This parameter can be one of the GPIO_PinState enum values:
 *            @arg GPIO_PIN_RESET: to clear the port pin
 *            @arg GPIO_PIN_SET: to set the port pin
 * @retval None
 */
void LCD_Init(SPI_HandleTypeDef *spi_handle, uint16_t reset_pin)
{
	// SPI must already be initialized
	assert_param(HAL_SPI_GetState(spi_handle) == HAL_SPI_STATE_READY);

	// Toggle reset pin low, then high
	HAL_GPIO_WritePin(GPIOA, reset_pin, GPIO_PIN_RESET);
	// TODO: small delay
	HAL_GPIO_WritePin(GPIOA, reset_pin, GPIO_PIN_SET);

}
