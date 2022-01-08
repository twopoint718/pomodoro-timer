/*
 * pcd8544.h
 *
 *  Created on: Jan 2, 2022
 *      Author: Adafruit (modified by Christopher Wilson <chris@sencjw.com>)
 */

#ifndef INC_PCD8544_H_
#define INC_PCD8544_H_

#define LCDWIDTH 84  // 84 pixels wide
#define LCDHEIGHT 48 // 48 pixels high

/*
 * PCD8544 Instruction Set (Basic, then Extended)
 *
 * | Instruction     | D/~C | Command Byte         | Description                  |
 * |-----------------+------+----------------------+------------------------------|
 * | NOP             |    0 | 0x00                 | no operation                 |
 * | Function set    |    0 | 0x20 OR 0x00         | chip active, horiz., basic   |
 * |                 |    0 | 0x20 OR 0x04         | power down                   |
 * |                 |    0 | 0x20 OR 0x02         | vertical addressing          |
 * |                 |    0 | 0x20 OR 0x01         | use extended instruction set |
 * | Write data      |    1 | 0x00..0xFF           | write data to RAM            |
 * | Display control |    0 | 0x08 OR 0x00         | Display blank                |
 * |                 |    0 | 0x08 OR 0x04         | Normal mode                  |
 * |                 |    0 | 0x08 OR 0x01         | All display segments on      |
 * |                 |    0 | 0x08 OR 0x05         | Inverse video mode           |
 * | Set Y addr RAM  |    0 | 0x40 OR (0x00..0x05) | Set Y addr of RAM 0 thru 5   |
 * | Set X addr RAM  |    0 | 0x80 OR (0x00..0x53) | Set X addr of RAM 0 thru 83  |
 * |-----------------|------|----------------------|------------------------------|
 * | Set bias        |    0 | 0x10 OR (0x00..0x07) | Set Bias System (BSx)        |
 * | Set Vop         |    0 | 0x80 OR (0x00..0x7F) | Set Vop (contrast)           |
 *
 */

// Function set
#define PCD8544_FUNCTIONSET 	0x20
#define PCD8544_POWERDOWN		0x04
#define PCD8544_VERTADDR		0x02
#define PCD8544_EXTINSTRUCTION	0x01

// Display Control
#define PCD8544_DISPLAYCONTROL	0x08
#define PCD8544_DISPLAYNORMAL	0x04
#define PCD8544_DISPLAYBLANK	0x00
#define PCD8544_DISPLAYALLON    0x01
#define PCD8544_DISPLAYINVERTED	0x05

// Set X address of RAM
#define PCD8544_SETXADDR		0x80

// Set Y address of RAM
#define PCD8544_SETYADDR		0x40

// Extended instructions
#define PCD8544_SETBIAS			0x10
#define PCD8544_SETVOP			0x80

void LCD_begin(SPI_HandleTypeDef *, GPIO_TypeDef *, uint16_t,
               uint16_t, uint16_t, uint8_t,
               uint8_t);
void LCD_clearDisplay();
void LCD_display();
void LCD_drawPixel(uint8_t, uint8_t, bool);
void LCD_initDisplay();
void LCD_setBias(uint8_t);
void LCD_setContrast(uint8_t);

#endif /* INC_PCD8544_H_ */
