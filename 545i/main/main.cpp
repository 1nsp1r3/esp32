#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <i3-lcd.h>

#define TAG "I3-MAIN"

#define BALL_SIZE 50

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  uint16_t *buffer = (uint16_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
  if (buffer != NULL) ESP_LOGI(TAG, "Initializing back buffer... OK");

  i3LcdInit();

  int x = 120;
  int incX = 1;
  int incY = 1;
  int y = 160;

  for(;;){
    i3LcdClear(buffer);
    i3LcdRectangle(buffer, x, y, x+BALL_SIZE-1, y+BALL_SIZE-1, LCD_COLOR_GREEN);
    //i3LcdSetPixel(buffer, x, y, LCD_COLOR_WHITE);
    i3LcdSwap(buffer);

    x+=incX;
    y+=incY;

    if (x >= LCD_WIDTH-BALL_SIZE) incX = -1;
    if (x <= 0) incX = 1;
    if (y >= LCD_HEIGHT-BALL_SIZE) incY = -1;
    if (y <= 0) incY = 1;

    //vTaskDelay (1000 / portTICK_PERIOD_MS);
    //ESP_LOGI(TAG, "Normalement, un dessin est apparu");
  }
}
