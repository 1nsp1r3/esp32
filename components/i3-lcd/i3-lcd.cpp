#include "i3-lcd.h"

/**
 *
 */
void i3LcdInit(){
  ESP_LOGV(I3_LCD_TAG, "i3LcdInit()");

  spi_bus_config_t buscfg = {
    .mosi_io_num = LCD_PIN_MOSI,
    .miso_io_num = LCD_PIN_MISO,
    .sclk_io_num = LCD_PIN_SCLK,
    .max_transfer_sz = 4000,
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
  ESP_LOGI(I3_LCD_TAG, "Initializing SPI bus... OK");

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
    .cs_gpio_num       = LCD_PIN_CS,
    .dc_gpio_num       = LCD_PIN_DC,
    .spi_mode          = 0,
    .pclk_hz           = (20 * 1000 * 1000), //80Mhz fonctionne à priori
    .trans_queue_depth = 10,
    .lcd_cmd_bits      = 8,
    .lcd_param_bits    = 8,
  };

  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));
  ESP_LOGI(I3_LCD_TAG, "Attaching LCD to the SPI bus... OK");

  esp_lcd_panel_dev_config_t panel_config = {
    .reset_gpio_num = -1,
    .rgb_endian = LCD_RGB_ELEMENT_ORDER_RGB,      //<- GROK
    //.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB, //<- MOI
    .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
  ESP_LOGI(I3_LCD_TAG, "Installing ST7789 panel driver... OK");

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  //ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
  //ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

  //Rétro-eclairage
  gpio_set_direction(LCD_PIN_BACK_LIGHT, GPIO_MODE_OUTPUT);
  gpio_set_level(LCD_PIN_BACK_LIGHT, 1);
}

/**
 * 
 */
void i3LcdClear(uint16_t *buffer){
  memset(buffer, LCD_COLOR_BLACK, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
}

/**
 * 
 */
void i3LcdSetPixel(uint16_t *buffer, int x, int y, uint16_t color){
  buffer[y * LCD_WIDTH + x] = color;
}

/**
 * 
 */
void i3LcdLineH(uint16_t *buffer, int x1, int x2, int y, uint16_t color){
  for (int x=x1;x<=x2;x++) buffer[y * LCD_WIDTH + x] = color;
}

/**
 * 
 */
void i3LcdRectangle(uint16_t *buffer, int x1, int y1, int x2, int y2, uint16_t color){
  for (int y=y1;y<=y2;y++) i3LcdLineH(buffer, x1, x2, y, color);
}

/**
 * 
 */
void i3LcdSwap(uint16_t *buffer){
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, buffer);  
}

/**
 * Draw a single digit (0-9) at the specified position
 */
void i3LcdDrawDigit(uint16_t *buffer, int x, int y, uint8_t digit, uint16_t color){
  if (digit > 9) return; // Only digits 0-9 are supported
  
  for (uint8_t col=0;col<5;col++){
    for (uint8_t row=0;row<7;row++){      
      uint8_t bit = font_digits[digit][col];
      if (bit & (1 << row)){ // Check if the pixel should be drawn (bit is set in font data)
        i3LcdSetPixel(buffer, x+col, y+row, color);
      }
    }
  }
}

/**
 * Draw a number at the specified position
 */
void i3LcdDrawNumber(uint16_t *buffer, int x, int y, int number, uint16_t color){
  char numStr[10]; //4294967295 (32bits unsigned)
  sprintf(numStr, "%d", number);
  
  int startX = x;
  for (int i=0;numStr[i]!='\0';i++){
    if (numStr[i] >= '0' && numStr[i] <= '9'){
      i3LcdDrawDigit(buffer, startX, y, numStr[i] - '0', color);
      startX += 8; // Move to the right for the next digit (5 pixels width + 3 pixels spacing)
    }
  }
}
