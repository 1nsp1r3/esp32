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
    .rgb_endian = LCD_RGB_ENDIAN_RGB,
    .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
  ESP_LOGI(I3_LCD_TAG, "Installing ST7789 panel driver... OK");

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  
  #ifdef LCD_LANDSCAPE_MODE
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true)); //Landscape mode

    //In landscape mode, the parameters order of esp_lcd_panel_mirror() is mirror_y then mirror_x
    //ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, true)); //usb-c power on the left
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));   //usb-c power on the right
  #else
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false)); //Portrait mode
    
    //In portrait mode, the parameters order of esp_lcd_panel_mirror() is mirror_x then mirror_y
    //ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, true)); //usb-c power on the top
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));  //usb-c power on the bottom
  #endif
  
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
void i3LcdSetPixel(uint16_t *buffer, uint16_t x, uint16_t y, uint16_t color){
  buffer[y * LCD_WIDTH + x] = color;
}

/**
 * 
 */
void i3LcdLineH(uint16_t *buffer, uint16_t x1, uint16_t x2, uint16_t y, uint16_t color){
  for (int x=x1;x<=x2;x++) buffer[y * LCD_WIDTH + x] = color;
}

/**
 * 
 */
void i3LcdRectangle(uint16_t *buffer, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
  for (int y=y1;y<=y2;y++) i3LcdLineH(buffer, x1, x2, y, color);
}

/**
 * 
 */
void i3LcdSwap(uint16_t *buffer){
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, buffer);  
}

/**
 * Draw a single char at the specified position with scaling
 */
void i3LcdChar(uint16_t *buffer, uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint8_t size) {
  int8_t character = c-32;
  if ((character < 0) || (character > 94)) return; // Protection
  if (size <= 0) size = 1; // Default size if invalid
  
  for (uint8_t col=0;col<5;col++){
    for (uint8_t row=0;row<7;row++){      
      uint8_t bit = font_5x7[character][col];
      if (bit & (1 << row)){ // Check if the pixel should be drawn (bit is set in font data)
        // Draw a scaled pixel (size x size block)
        for (int sy = 0; sy < size; sy++) {
          for (int sx = 0; sx < size; sx++) {
            i3LcdSetPixel(buffer, x + col * size + sx, y + row * size + sy, color);
          }
        }
      }
    }
  }
}

/**
 * Display a text at the specified position with scaling
 */
void i3LcdString(uint16_t *buffer, uint16_t x, uint16_t y, char* text, uint16_t color, uint8_t size) {
  int startX = x;
  for (int i=0;text[i]!='\0';i++){
    i3LcdChar(buffer, startX, y, text[i], color, size);
    startX += 3+5*size; // Move to the right for the next digit (5 pixels width + 3 pixels spacing, scaled)
  }
}

/**
 * Display a sprite
 */
void i3LcdSprite(uint16_t *buffer, uint8_t *sprite, uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint8_t size){
  for (uint8_t col=0;col<width;col++){
    for (uint8_t row=0;row<height;row++){      
      uint8_t pixel = sprite[row*width+col];
      uint16_t color = 0xF0FF & pixel;

      switch(pixel){
        case 0x3C : color = 0x0000;break; //Background color
        //case 0x3C : color = 0xC637;break; //green
        case 0xE0 : color = 0x00F8;break; //red
        case 0xFF : color = 0xFFFF;break; //white
        case 0x03 : color = 0x1C00;break; //dark blue
        case 0x13 : color = 0x1F04;break; //light blue
        case 0xA0 : color = 0x00A0;break; //marron
        case 0xF6 : color = 0x51E5;break; //beige
        case 0xAD : color = 0xCBAB;break; //saumon

        case 0x29 : color = 0x492A;break; //brun
        case 0x4D : color = 0x492A;break;

        case 0xDF : color = 0x5DCF;break; //blanc cassé
        case 0xD0 : color = 0xC4DC;break; //orange
        case 0x60 : color = 0x0078;break; //bordeau
        case 0x6D : color = 0xCF7B;break; //gris
        

        case 0x04 : color = 0x0000;break; //black
      }

      //i3LcdSetPixel(buffer, x + col, y + row, color);
      for (int sy = 0; sy < size; sy++) {
        for (int sx = 0; sx < size; sx++) {
          i3LcdSetPixel(buffer, x + col * size + sx, y + row * size + sy, color);
        }
      }
    }
  }
}
