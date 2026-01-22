#include "i3-lcd.h"

/**
 *
 */
uint16_t* i3LcdInit(bool LandscapeMode, bool MirrorMode){
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
    .rgb_endian = LCD_RGB_ENDIAN_RGB,
    .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
  ESP_LOGI(I3_LCD_TAG, "Installing ST7789 panel driver... OK");

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

  if (LandscapeMode){
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true)); //Landscape mode
    lcdWidth = 320;
    lcdHeight = 240;    
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, !MirrorMode, MirrorMode)); //In landscape mode, the parameters order of esp_lcd_panel_mirror() is mirror_y then mirror_x
  }else{
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false)); //Portrait mode
    lcdWidth = 240;
    lcdHeight = 320;    
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, MirrorMode, MirrorMode)); //In portrait mode, the parameters order of esp_lcd_panel_mirror() is mirror_x then mirror_y
  }

  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

  //Rétro-eclairage
  gpio_set_direction(LCD_PIN_BACK_LIGHT, GPIO_MODE_OUTPUT);
  gpio_set_level(LCD_PIN_BACK_LIGHT, 1);

  lcdBuffer = (uint16_t*)malloc(lcdWidth * lcdHeight * sizeof(uint16_t));
  if (lcdBuffer == NULL) ESP_LOGE(I3_LCD_TAG, "Initializing back buffer... KO");
  return lcdBuffer;
}

/**
 * 
 */
void i3LcdClear(uint16_t* buffer){
  uint16_t* b = (buffer == NULL) ? lcdBuffer : buffer;
  memset(b, LCD_COLOR_BLACK, lcdWidth * lcdHeight * sizeof(uint16_t));
}

/**
 * Not compatible with the lcd buffer
 */
uint8_t i3LcdGetPixel(uint16_t x, uint16_t y, uint16_t width, uint8_t* buffer){
  return buffer[y*width+x];
}

/**
 *
 */
void i3LcdSetPixel(uint16_t x, uint16_t y, uint16_t color, uint16_t* buffer){
  uint16_t* b = (buffer == NULL) ? lcdBuffer : buffer;
  b[y * lcdWidth + x] = color;
}

/**
 *
 */
void i3LcdLineH(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color, uint16_t* buffer){
  uint16_t* b = (buffer == NULL) ? lcdBuffer : buffer;
  for (int x=x1;x<=x2;x++) b[y * lcdWidth + x] = color;
}

/**
 *
 */
void i3LcdRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t* buffer){
  for (int y=y1;y<=y2;y++) i3LcdLineH(x1, x2, y, color, buffer);
}

/**
 *
 */
void i3LcdSwap(uint16_t* buffer){
  uint16_t* b = (buffer == NULL) ? lcdBuffer : buffer;
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, lcdWidth, lcdHeight, b);
}

/**
 * Draw a single char at the specified position with scaling
 */
void i3LcdChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint8_t size, uint16_t* buffer) {
  int8_t character = c-32;
  if ((character < 0) || (character > 94)) return; // Protection
  if (size <= 0) size = 1; // Default size if invalid

  for (uint8_t col=0;col<5;col++){
    for (uint8_t row=0;row<7;row++){
      uint8_t bit = font_5x7[character][col];
      if (bit & (1 << row)){ // Check if the pixel should be drawn (bit is set in font data)
        // Draw a scaled pixel (size x size block)
        for (uint16_t sy = 0; sy < size; sy++) {
          for (uint16_t sx = 0; sx < size; sx++) {
            i3LcdSetPixel(x + col * size + sx, y + row * size + sy, color, buffer);
          }
        }
      }
    }
  }
}

/**
 * Display a text at the specified position with scaling
 */
void i3LcdString(uint16_t x, uint16_t y, char* text, uint16_t color, uint8_t size, uint16_t* buffer) {
  uint16_t startX = x;
  for (uint16_t i=0;text[i]!='\0';i++){
    i3LcdChar(startX, y, text[i], color, size, buffer);
    startX += 3+5*size; // Move to the right for the next digit (5 pixels width + 3 pixels spacing, scaled)
  }
}

/**
 * Display a text at the specified position with scaling
 */
uint16_t i3LcdLength(const char* text, uint8_t size){
    int len = strlen(text);
    int lenChar  = len*5*size;
    int lenSpace = (len-1)*3;
    return lenChar + lenSpace;
}

/**
 *
 */
uint16_t i3LcdToRgb565(uint8_t r, uint8_t g, uint8_t b){
  uint16_t tmp = (uint16_t)((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
  uint8_t h = tmp >> 8;
  uint8_t l = tmp & 0x00FF;
  return (l << 8) | h;
}

/**
 *
 */
uint16_t i3LcdGetColorFromPalette(uint8_t palette[][3], uint8_t index){
  return i3LcdToRgb565(palette[index][0], palette[index][1], palette[index][2]);
}



/**
 * Display a sprite
 */
void i3LcdSprite(uint8_t *sprite, uint8_t palette[][3], uint16_t srcX, uint16_t srcY, uint16_t srcWidth, uint16_t dstX, uint16_t dstY, uint16_t width, uint16_t height, bool xReverse, uint8_t size, bool transparency, uint16_t* buffer){
  for (uint16_t col=0;col<width;col++){
    for (uint16_t row=0;row<height;row++){
      uint8_t colorIndex = i3LcdGetPixel(srcX+col, srcY+row, srcWidth, sprite);
      if (transparency && (colorIndex == 0)) continue;
      uint16_t color = i3LcdGetColorFromPalette(palette, colorIndex);

      uint16_t x = xReverse ? (dstX+width-1)-col : dstX+col;
      i3LcdSetPixel(x, dstY+row, color, buffer);
      /*for (uint16_t sy = 0; sy < size; sy++) {
        for (uint16_t sx = 0; sx < size; sx++) {
          i3LcdSetPixel(xDst + col * size + sx, yDst + row * size + sy, color, buffer);
        }
      }*/
    }
  }
}
