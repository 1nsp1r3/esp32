#include "i3-lcd.h"

/**
 *
 */
void i3LcdSetBrightnessLevel(uint8_t Level){
  if (Level > 100){
    ESP_LOGE(I3_LCD_TAG, "Brightness value out of range");
    return;
  }

  uint32_t duty = (Level * (LCD_BL_LEDC_DUTY - 1)) / 100;

  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
  ESP_LOGI(I3_LCD_TAG, "LCD brightness set to %d%%", Level);
}

/**
 *
 */
void i3LcdInit(){
  ESP_LOGV(I3_LCD_TAG, "i3LcdInit()");

  spi_bus_config_t buscfg = {
    .mosi_io_num = PIN_NUM_LCD_MOSI,
    .miso_io_num = PIN_NUM_LCD_MISO,
    .sclk_io_num = PIN_NUM_LCD_SCLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4000,
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
  ESP_LOGI(I3_LCD_TAG, "Initializing SPI bus... OK");

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
    .cs_gpio_num       = PIN_NUM_LCD_CS,
    .dc_gpio_num       = PIN_NUM_LCD_DC,
    .spi_mode          = 0,
    .pclk_hz           = (80 * 1000 * 1000),
    .trans_queue_depth = 10,
    .lcd_cmd_bits      = 8,
    .lcd_param_bits    = 8,
    //.on_color_trans_done = //For LVGL
  };

  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));
  ESP_LOGI(I3_LCD_TAG, "Attaching LCD to the SPI bus... OK");

  esp_lcd_panel_dev_config_t panel_config = {
    .reset_gpio_num = -1,
    .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
    .bits_per_pixel = 16,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
  ESP_LOGI(I3_LCD_TAG, "Installing ST7789 panel driver... OK");

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
  ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

  //Brightness_init
  gpio_set_direction(PIN_NUM_BK_LIGHT, GPIO_MODE_OUTPUT);
  gpio_set_level(PIN_NUM_BK_LIGHT, 1);

  ledc_timer_config_t ledc_timer = {
    .speed_mode      = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_10_BIT,  // Set duty resolution to 13 bits,
    .timer_num       = LEDC_TIMER_0,
    .freq_hz         = 10000,              // Set output frequency at 5 kHz
    .clk_cfg         = LEDC_AUTO_CLK,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
  ESP_LOGI(I3_LCD_TAG, "Initializing LEDC timer... OK");

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {
    .gpio_num   = PIN_NUM_BK_LIGHT,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = LEDC_CHANNEL_0,
    .intr_type  = LEDC_INTR_DISABLE,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = 0, // Set duty to 0%
    .hpoint     = 0,
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
  ESP_LOGI(I3_LCD_TAG, "Initializing LEDC channel... OK");

  i3LcdSetBrightnessLevel(80);
}
