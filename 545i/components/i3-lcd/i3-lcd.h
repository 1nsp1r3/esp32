#ifndef I3_LCD_H
#define I3_LCD_H

#define I3_LCD_TAG "I3-LCD"

#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <driver/spi_master.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>
#include "driver/gpio.h"

#define LCD_PIN_BACK_LIGHT GPIO_NUM_1
#define LCD_PIN_MOSI       38
#define LCD_PIN_SCLK       39
#define LCD_PIN_MISO       40
#define LCD_PIN_DC         42
#define LCD_PIN_CS         45

#define LCD_WIDTH  240
#define LCD_HEIGHT 320

//Color format: RGB565 (16 bits)
#define LCD_COLOR_RED   0x07E0
#define LCD_COLOR_GREEN 0x001F
#define LCD_COLOR_BLUE  0xF800
#define LCD_COLOR_WHITE 0xFFFF
#define LCD_COLOR_BLACK 0x0000

static esp_lcd_panel_handle_t panel_handle;

// Police 5x7
// See https://github.com/M4nu3k3/GTO/blob/14023d8f88590e13da40ea48a4ab150c495002a8/GTO_STM32/core/Src/fonts.c for another characters
static const uint8_t font_digits[10][5] = {
  {0x3E,0x51,0x49,0x45,0x3E}, //0
  {0x00,0x42,0x7F,0x40,0x00}, //1
  {0x42,0x61,0x51,0x49,0x46}, //2
  {0x21,0x41,0x45,0x4B,0x31}, //3
  {0x18,0x14,0x12,0x7F,0x10}, //4
  {0x27,0x45,0x45,0x45,0x39}, //5
  {0x3C,0x4A,0x49,0x49,0x30}, //6
  {0x01,0x71,0x09,0x05,0x03}, //7
  {0x36,0x49,0x49,0x49,0x36}, //8
  {0x06,0x49,0x49,0x29,0x1E}  //9
};

void i3LcdInit();
void i3LcdClear(uint16_t *buffer);
void i3LcdSetPixel(uint16_t *buffer, int x, int y, uint16_t color);
void i3LcdLineH(uint16_t *buffer, int x1, int x2, int y, uint16_t color);
void i3LcdRectangle(uint16_t *buffer, int x1, int y1, int x2, int y2, uint16_t color);
void i3LcdSwap(uint16_t *buffer);
void i3LcdDrawDigit(uint16_t *buffer, int x, int y, uint8_t digit, uint16_t color);
void i3LcdDrawNumber(uint16_t *buffer, int x, int y, int number, uint16_t color);

#endif
