#include <time.h>
#include <i3-lcd.h>
#include <i3-lzw.h>
#include <i3-sys.h>
#include <i3-sprite.h>

#include "asset/mario.h"
#include "animation/marioScreenBorder.h"
#include "animation/marioRotation.h"

#define TAG "I3-MAIN"

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  uint8_t* sprites = i3SysPsramMallocUint8(MARIO_WIDTH*MARIO_HEIGHT);
  i3Unzip(marioData, sizeof(marioData), sprites);
  i3SpriteInit(sprites, MARIO_WIDTH, MARIO_HEIGHT, marioPalette);
  
  i3LcdInit();

  for(;;){
    i3LcdClear(); 
    
    marioScreenBorderPlayFrame();
    marioRotationPlayFrame();

    i3LcdSwap();
    vTaskDelay (10 / portTICK_PERIOD_MS);
  }
}
