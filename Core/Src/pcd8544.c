/*
 * pcd8544.c
 *
 *  Created on: Jan 2, 2022
 *      Author: cjw
 */

#include "main.h"

static uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {0};
static uint16_t buffsize = sizeof(pcd8544_buffer);

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

// Private module variables
static GPIO_TypeDef *_port;
static uint8_t _bias;
static uint8_t _contrast;
// static uint8_t _reinit_interval;
static uint8_t _display_count;

// Pins
static uint16_t _dcPin;
static uint16_t _resetPin;

static SPI_HandleTypeDef *_hspi;

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
static void command(uint8_t command) {
  HAL_GPIO_WritePin(_port, _dcPin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(_hspi, &command, 1, HAL_MAX_DELAY);
}

/*****************************************************************************/

/**
 * @brief Create and initialize the LCD module. Both D/C and Reset pins must
 * be on the same port. Call this function first!
 * @param hspi an already initialize SPI struct
 * @param port a pointer to the GPIO port struct where all the remaining GPIO
 * pins are: D/C and Reset
 * @param dcPin the Data/~Command pin, when low it means we're sending commands
 * to the LCD. When the pin is high we're setting a byte in RAM at the current
 * address
 * @param resetPin a reset (~RES) pulse (negative low) is used to initialize the
 * LCD
 * @param contrast set the contrast of the display
 * @param bias sets the bias
 * @retval None
 */
void LCD_begin(SPI_HandleTypeDef *hspi, GPIO_TypeDef *port, uint16_t dcPin,
               uint16_t resetPin, uint8_t contrast,
               uint8_t bias) {
  _bias = bias;
  _contrast = contrast;
  // _reinit_interval = 0;
  _display_count = 0;

  _port = port;
  _dcPin = dcPin;
  _resetPin = resetPin;
  _hspi = hspi;

  LCD_initDisplay();
}

void LCD_clearDisplay() {
  memset(pcd8544_buffer, 0, buffsize);
}

/**
 * For simplicity, set the whole display in one go. LCD_drawPixel takes care of
 * transforming the pixel coordinates so that they will be in the correct
 * orientation in the buffer (LSB is the top (zeroth) row, MSB is 7th row).
 * X coordinates proceed across and Y down.
 */
void LCD_display() {
  command(PCD8544_SETYADDR);	// set to y = 0
  command(PCD8544_SETXADDR);	// set to x = 0

  HAL_GPIO_WritePin(_port, _dcPin, GPIO_PIN_SET);
  HAL_SPI_Transmit(_hspi, pcd8544_buffer, buffsize, HAL_MAX_DELAY); // TODO: delay

  command(PCD8544_SETYADDR); // no idea why this is necessary but it is to
                             // finish the last byte?
}

void LCD_drawPixel(int16_t x, int16_t y, bool color) {
  if (color)
    pcd8544_buffer[x + (y / 8) * LCDWIDTH] |= 1 << (y % 8);
  else
    pcd8544_buffer[x + (y / 8) * LCDWIDTH] &= ~(1 << (y % 8));
}

/**
 * @brief Initialize the PCD8544: 48 × 84 pixels matrix LCD Set bias and
 * contrast, enter normal mode.
 *
 * @param  GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4
 *         family
 * @param  GPIO_Pin specifies the port bit to be written.
 *         This parameter can be any combination of GPIO_Pin_x where x can be
 * (0..15).
 * @retval None
 */
void LCD_initDisplay() {
  if (HAL_GPIO_ReadPin(_port, _resetPin) >= 0) {
    HAL_GPIO_WritePin(_port, _resetPin, GPIO_PIN_RESET); // set reset pin low
    HAL_Delay(10);                                        // delay
    HAL_GPIO_WritePin(_port, _resetPin, GPIO_PIN_SET);   // set high
  }

  LCD_setBias(_bias);
  LCD_setContrast(_contrast);

  command(PCD8544_FUNCTIONSET); // active, horiz. addressing, basic command set
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL); // display normal
}

void LCD_invertDisplay() {
  command(PCD8544_FUNCTIONSET);
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED);
}

void LCD_setBias(uint8_t bias) {
  uint8_t val = bias & 0x07;
  _bias = bias;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTINSTRUCTION); // extended instr. mode
  command(PCD8544_SETBIAS | val);                        // set bias
  command(PCD8544_FUNCTIONSET);                          // basic instr. mode
}

void LCD_setContrast(uint8_t contrast) {
  uint8_t val = contrast & 0x7F;
  _contrast = contrast;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTINSTRUCTION); // extended instr. mode
  command(PCD8544_SETVOP | val);                         // set Vop (contrast)
  command(PCD8544_FUNCTIONSET);                          // basic instr. mode
}
