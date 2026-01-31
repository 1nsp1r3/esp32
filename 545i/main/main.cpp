#include <i3-lcd.h>
#include <i3-lzw.h>
#include <i3-sys.h>
#include <i3-sprite.h>
#include "timer.h"
#include "adc.h"
#include "logo-bmw.h"

#define TAG "I3-MAIN"

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  //-------------
  //Splashscreen
  //-------------
  uint8_t* splashscreen = i3SysPsramMallocUint8(LOGO_WIDTH*LOGO_HEIGHT); //PSRAM alloc to relieve RAM occupation
  i3Unzip(logo, sizeof(logo), splashscreen);
  i3SpriteInit(splashscreen, LOGO_WIDTH, LOGO_HEIGHT, palette);

  //-------------
  //LCD
  //-------------
  uint16_t tempLength   = i3LcdLength("---", 20);
  uint16_t timerLength  = i3LcdLength("00:00:00", 4);
  uint16_t minMaxLength = i3LcdLength("---", 1);

  i3LcdInit(
    true, //LandscapeMode
    true  //MirrorMode
  );
  i3LcdClear();
  i3SpriteDraw({0, 0}, {60, 20}, {LOGO_WIDTH, LOGO_HEIGHT});
  i3LcdSwap(); //Show BMW logo

  adcInit();
  timerInit();

  uint16_t count = 0;
  for(;;){
    timerProcess();
    if (count++ % 2 == 0) adcProcess(); //Every 2s, read adc

    if (count > 5){ //After 5s, hide the BMW logo and display informations
      i3LcdClear();
      i3LcdString({(uint16_t)(160-(timerLength/2)), 209},       timer, LCD_COLOR_GREEN, 4);  //209 = 239-(7x4)-2
      i3LcdString({ (uint16_t)(160-(tempLength/2)),  35}, temperature, LCD_COLOR_WHITE, 20); //35 = (210/2)-((7x20)/2)
      i3LcdString({                              2, 232},     tempMin, LCD_COLOR_WHITE, 1);
      i3LcdString({   (uint16_t)(318-minMaxLength), 232},     tempMax, LCD_COLOR_WHITE, 1);
      i3LcdSwap();
    }
    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
