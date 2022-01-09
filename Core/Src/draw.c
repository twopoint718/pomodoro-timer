#include "main.h"

// Adafruit implementation
void draw_circle_af(int x0, int y0, int r) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    // filling in 4 quadrants at once, all symmetric
    LCD_drawPixel(x0, y0 - r, 1);  // N
    LCD_drawPixel(x0 - r, y0, 1);  // W
    LCD_drawPixel(x0, y0 + r, 1);  // S
    LCD_drawPixel(x0 + r, y0, 1);  // E

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
        LCD_drawPixel(x0 - x, y0 - y, 1);  // NNW
        LCD_drawPixel(x0 - y, y0 - x, 1);  // WNW
        LCD_drawPixel(x0 - y, y0 + x, 1);  // WSW
        LCD_drawPixel(x0 - x, y0 + y, 1);  // SSW
        LCD_drawPixel(x0 + x, y0 + y, 1);  // SSE
        LCD_drawPixel(x0 + y, y0 + x, 1);  // ESE
        LCD_drawPixel(x0 + y, y0 - x, 1);  // ENE
        LCD_drawPixel(x0 + x, y0 - y, 1);  // NNE
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
            di = di + 4 * xi + 6;           // see derivation [1]
            xi = xi + 1;
            // yi remains the same
        } else {
            di = di + 4 * (xi - yi) + 10;   // see derivation [2]
            xi = xi + 1;
            yi = yi - 1;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1) {
    _Bool steep = abs(y1 - y0) > abs(x1 - x0);
    int tmp;

    if (steep) {
        tmp = x0; x0 = y0; y0 = tmp; // swap x0, y0
        tmp = x1; x1 = y1; y1 = tmp; // swap x1, y1
    }

    if (x0 > x1) {
        tmp = x0; x0 = x1; x1 = tmp; // swap x0, x1
        tmp = y0; y0 = y1; y1 = tmp; // swap y0, y1
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
