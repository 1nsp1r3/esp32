#ifndef I3_LCD_H
#define I3_LCD_H

#define I3_LCD_TAG "I3-LCD"

#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <driver/spi_master.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include "driver/gpio.h"
#include "i3-lcd-5x7.h"

#define LCD_LANDSCAPE_MODE "landscape"

#ifdef LCD_LANDSCAPE_MODE
    #define LCD_WIDTH  320
    #define LCD_HEIGHT 240
#else
    #define LCD_WIDTH  240
    #define LCD_HEIGHT 320
#endif

#define LCD_PIN_BACK_LIGHT GPIO_NUM_1
#define LCD_PIN_MOSI       38
#define LCD_PIN_SCLK       39
#define LCD_PIN_MISO       40
#define LCD_PIN_DC         42
#define LCD_PIN_CS         45

//Color format: RGB565 (16 bits)
#define LCD_COLOR_RED   0x07E0
#define LCD_COLOR_GREEN 0x001F
#define LCD_COLOR_BLUE  0xF800
#define LCD_COLOR_WHITE 0xFFFF
#define LCD_COLOR_BLACK 0x0000

static esp_lcd_panel_handle_t panel_handle;

void i3LcdInit();
void i3LcdClear(uint16_t *buffer);
void i3LcdSetPixel(uint16_t *buffer, uint16_t x, uint16_t y, uint16_t color);
void i3LcdLineH(uint16_t *buffer, uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);
void i3LcdRectangle(uint16_t *buffer, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void i3LcdSwap(uint16_t *buffer);
void i3LcdChar(uint16_t *buffer, uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint8_t size);
void i3LcdString(uint16_t *buffer, uint16_t x, uint16_t y, char *text, uint16_t color, uint8_t size);
void i3LcdSprite(uint16_t *buffer, uint8_t *sprite, uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint8_t size);

#endif
