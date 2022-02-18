#include "glcdfont.c"
#include "font8x8_basic.h"
#include "pcd8544.h"

// Adafruit implementation
void draw_circle_af(int x0, int y0, int r) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  // filling in 4 quadrants at once, all symmetric
  LCD_drawPixel(x0, y0 - r, 1); // N
  LCD_drawPixel(x0 - r, y0, 1); // W
  LCD_drawPixel(x0, y0 + r, 1); // S
  LCD_drawPixel(x0 + r, y0, 1); // E

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    // in CCW order
    LCD_drawPixel(x0 - x, y0 - y, 1); // NNW
    LCD_drawPixel(x0 - y, y0 - x, 1); // WNW
    LCD_drawPixel(x0 - y, y0 + x, 1); // WSW
    LCD_drawPixel(x0 - x, y0 + y, 1); // SSW
    LCD_drawPixel(x0 + x, y0 + y, 1); // SSE
    LCD_drawPixel(x0 + y, y0 + x, 1); // ESE
    LCD_drawPixel(x0 + y, y0 - x, 1); // ENE
    LCD_drawPixel(x0 + x, y0 - y, 1); // NNE
  }
}

void draw_circle(unsigned int x, unsigned int y, unsigned int r) {
  int di = 3 - 2 * r; // initial 'decision parameter', see derivation [0]
  int xi = 0;
  int yi = r;

  while (xi <= yi) {
    LCD_drawPixel(xi + x, yi + y, 1);
    LCD_drawPixel(x - xi, yi + y, 1);
    LCD_drawPixel(xi + x, y - yi, 1);
    LCD_drawPixel(x - xi, y - yi, 1);

    LCD_drawPixel(yi + x, xi + y, 1);
    LCD_drawPixel(x - yi, y + xi, 1);
    LCD_drawPixel(x + yi, y - xi, 1);
    LCD_drawPixel(x - yi, y - xi, 1);
    if (di <= 0) {
      di = di + 4 * xi + 6; // see derivation [1]
      xi = xi + 1;
      // yi remains the same
    } else {
      di = di + 4 * (xi - yi) + 10; // see derivation [2]
      xi = xi + 1;
      yi = yi - 1;
    }
  }
}

void draw_line(int x0, int y0, int x1, int y1) {
  _Bool steep = abs(y1 - y0) > abs(x1 - x0);
  int tmp;

  if (steep) {
    tmp = x0;
    x0 = y0;
    y0 = tmp; // swap x0, y0
    tmp = x1;
    x1 = y1;
    y1 = tmp; // swap x1, y1
  }

  if (x0 > x1) {
    tmp = x0;
    x0 = x1;
    x1 = tmp; // swap x0, x1
    tmp = y0;
    y0 = y1;
    y1 = tmp; // swap y0, y1
  }

  int dx = x1 - x0;
  int dy = abs(y1 - y0);

  int err = dx / 2;
  int ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      LCD_drawPixel(y0, x0, 1);
    } else {
      LCD_drawPixel(x0, y0, 1);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void draw_char(int16_t x, int16_t y, unsigned char c, uint8_t color) {
    if ((x >= LCDWIDTH) || (y >= LCDHEIGHT) || (x < 0) || (y < 0)) // clip
        return;

    for (int8_t i = 0; i < 5; i++) {                // Char bitmap = 5 columns
        uint8_t line = font[c * 5 + i];             // Get ith col of font glyph
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                LCD_drawPixel(x + i, y + j, color);
            }
        }
    }
}

/*******************************************************************************
 * Draw an 8x8 character
 ******************************************************************************/
void draw_char8x8(int16_t x, int16_t y, unsigned char c, unsigned char color) {
    if ((x >= LCDWIDTH) || (y >= LCDHEIGHT) || (x < 0) || (y < 0)) // clip
        return;

    for (int8_t j = 0; j < 8; j++) {
        char row = font8x8_basic[c][j];
        for (int8_t i = 0; i < 8; i++, row >>= 1)
            if (row & 1)
                LCD_drawPixel(x + i, y + j, color);
    }
}

/*******************************************************************************
 * Draw a buffer of characters as contiguous text. Using the LCDWIDTH and
 * LCDHEIGHT parameters, attempt to fit the text within the screen area. Any \n
 * characters in the text get turned into linebreaks within the drawn text.
 ******************************************************************************/
void draw_string(uint8_t x, uint8_t y, const char *msg, int size) {
    int row = 0;
    int col = 0;
    int fill_col = LCDWIDTH/8;
    int fill_row = LCDHEIGHT/8;
    for (int i = 0; i < size; i++, col++) {
      if (row >= fill_row) break;
      if (col >= fill_col || msg[i] == '\n') {
        row++;
        col = 0;
        if (msg[i] == '\n') continue; // skip the actual \n character
      }
      draw_char8x8(col * 8 + x, row * 8 + y, msg[i], 1);
    }
}

/*******************************************************************************
 * Draw a right-justified, up to 3 digit number at the location specified.
 ******************************************************************************/
void draw_number(uint8_t x, uint8_t y, uint8_t num) {
    if (num == 0)
        draw_char8x8(x + 16, y, '0', 1);
    int xpos = x + 16;
    while (num > 0) {
        int d = num % 10;
        draw_char8x8(xpos, y, d + '0', 1);
        num /= 10;
        xpos -= 8;
    }
}

/*******************************************************************************
 * Draw a right-justified, up to 5 digit number at the location specified.
 ******************************************************************************/
void draw_number16(uint8_t x, uint8_t y, int16_t num) {
    if (num == 0)
        draw_char8x8(x + 6*8, y, '0', 1);
    int xpos = x + 6*8;
    if (num < 0) {
        draw_char8x8(x, y, '-', 1);
        num = num * -1;
    }
    while (num > 0) {
        int d = num % 10;
        draw_char8x8(xpos, y, d + '0', 1);
        num /= 10;
        xpos -= 8;
    }
}

void draw_clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    int i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            LCD_drawPixel(i + x, j + y, 0);
}


