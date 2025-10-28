#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#include "driver/gpio.h"

#include <i3-lcd.h>

#define TAG "I3-MAIN"

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  i3LcdInit();

  uint16_t *buffer = (uint16_t*)heap_caps_malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA);
  memset(buffer, 0x0000, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

  /*
  const uint16_t white = 0x00FF; // Couleur blanche en RGB565
  for (int y = 50; y < 55; y++) {
      for (int x = 5; x < LCD_WIDTH-5; x++) {
          buffer[y * LCD_WIDTH + x] = white; // Dessine un rectangle blanc comme pseudo-texte
      }
  }*/

  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 10, 10, buffer);

  for(;;){
    vTaskDelay (1000 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "Normalement, un dessin est apparu");
  }
}
