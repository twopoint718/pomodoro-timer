/*
 * pcd8544.h
 *
 *  Created on: Jan 2, 2022
 *      Author: Adafruit
 */

#ifndef INC_PCD8544_H_
#define INC_PCD8544_H_

#define WIDTH 84  // 84 pixels wide
#define HEIGHT 48 // 48 pixels high

#define PCD8544_FUNCTIONSET 	0x20
#define PCD8544_DISPLAYCONTROL	0x08
#define PCD8544_DISPLAYNORMAL	0x04

void LCD_Init(SPI_HandleTypeDef *);
void Command(SPI_HandleTypeDef *, uint8_t);

#endif /* INC_PCD8544_H_ */
