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

//Color format: RGB565 (16 bits)
#define LCD_COLOR_RED   0x07E0
#define LCD_COLOR_GREEN 0x001F
#define LCD_COLOR_BLUE  0xF800
#define LCD_COLOR_WHITE 0xFFFF
#define LCD_COLOR_BLACK 0x0000

#define LCD_PIN_BACK_LIGHT GPIO_NUM_1
#define LCD_PIN_MOSI       38
#define LCD_PIN_SCLK       39
#define LCD_PIN_MISO       40
#define LCD_PIN_DC         42
#define LCD_PIN_CS         45

static esp_lcd_panel_handle_t panel_handle;
static uint16_t* lcdBuffer = NULL;
static uint16_t lcdWidth  = 0; //See i3LcdInit()
static uint16_t lcdHeight = 0; //See i3LcdInit()

typedef struct {
    uint16_t x;
    uint16_t y;
} Pos;

uint16_t* i3LcdInit(bool LandscapeMode=true, bool MirrorMode=false);
void i3LcdClear(uint16_t* buffer=NULL);
void i3LcdSetPixel(Pos pos, uint16_t color, uint16_t* buffer=NULL);
void i3LcdLineH(Pos src, Pos dst, uint16_t color, uint16_t* buffer=NULL);
void i3LcdLineV(Pos src, Pos dst, uint16_t color, uint16_t* buffer=NULL);
void i3LcdRectangle(Pos topLeft, Pos bottomRight, uint16_t color, uint16_t* buffer=NULL);
void i3LcdSwap(uint16_t* buffer=NULL);
void i3LcdChar(Pos pos, uint8_t c, uint16_t color, uint8_t size, uint16_t* buffer=NULL);
void i3LcdString(Pos pos, char* text, uint16_t color, uint8_t size, uint16_t* buffer=NULL);
uint16_t i3LcdLength(const char* text, uint8_t size);
uint16_t i3LcdToRgb565(uint8_t r, uint8_t g, uint8_t b);
uint16_t i3LcdGetColorFromPalette(uint8_t palette[][3], uint8_t index);

#endif
