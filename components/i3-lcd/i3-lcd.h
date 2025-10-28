#ifndef I3_LCD_H
#define I3_LCD_H

#define I3_LCD_TAG "I3-LCD"

#include <esp_log.h>
#include <esp_lcd_panel_io.h>
#include <driver/spi_master.h>
#include <driver/ledc.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_ops.h>

#define PIN_NUM_BK_LIGHT GPIO_NUM_1
#define PIN_NUM_LCD_MOSI 38
#define PIN_NUM_LCD_SCLK 39
#define PIN_NUM_LCD_MISO 40
#define PIN_NUM_LCD_DC   42
#define PIN_NUM_LCD_CS   45

#define LCD_BL_LEDC_DUTY (1024) // Set duty to 50%. (2 ** 13) * 50% = 4096

#define LCD_WIDTH  240
#define LCD_HEIGHT 320

static esp_lcd_panel_handle_t panel_handle;

void i3LcdInit();
void i3LcdSetBrightnessLevel(uint8_t Level);

#endif
