#include <time.h>
#include <i3-lcd.h>
#include <i3-lzw.h>
#include "logo-mazda.h"

#define TAG "I3-MAIN"

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  unsigned char* data = i3Unzip(logo, sizeof(logo), LOGO_WIDTH*LOGO_HEIGHT);

  i3LcdInit();
  i3LcdClear();
  i3LcdSprite(data, palette, 0, 0, LOGO_WIDTH, LOGO_HEIGHT);
  i3LcdSwap();

  for(;;){
    vTaskDelay (5000 / portTICK_PERIOD_MS);
  }
}
