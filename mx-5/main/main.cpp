#include <i3-adc.h>
#include <i3-lcd.h>
#include <i3-lzw.h>
#include <i3-sys.h>
#include <i3-sprite.h>
#include "timer.h"
#include "temperature.h"
#include "pressure.h"
#include "asset/logo-mazda.h"

#define TAG "I3-MAIN"

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG, ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)

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
  uint16_t temperatureLength = i3LcdLength("---", 18);
  uint16_t pressureLength    = i3LcdLength("---", 10);
  uint16_t minMaxLength      = i3LcdLength("---", 1);

  i3LcdInit(
    true, //LandscapeMode
    false  //MirrorMode
  );
  i3LcdClear();
  i3SpriteDraw({0, 0}, {9, 0}, {LOGO_WIDTH, LOGO_HEIGHT});
  i3LcdSwap(); //Show logo

  i3AdcInit(ADC_UNIT_1);
  i3AdcInitChannel(ADC_CHANNEL_1);
  i3AdcInitChannel(ADC_CHANNEL_3);
  timerInit();

  uint16_t LCD_COLOR_OIL  = i3LcdToRgb565(245, 203, 95);
  uint16_t LCD_COLOR_GRAY = i3LcdToRgb565(61, 61, 61);

  uint16_t count = 0;
  bool splahScreenFinished = false;
  for(;;){
    count++;
    if (count % 10){        //every 1s
      timerProcess();
      pressureProcess();
    }else if (count % 20) { //every 2s
      temperatureProcess();
    }
    if (count > 50){
      splahScreenFinished = true; //Hide logo, display information
    }

    if (splahScreenFinished){
      i3LcdClear();
      i3LcdString({                                            0, 225},          timer,  LCD_COLOR_BLUE,  2); //Total height=7x2=14 pixels
                                                                                                              //239-14=225

      i3LcdString({                   (uint16_t)(minMaxLength+5),   0},    temperature, LCD_COLOR_WHITE, 18); //Total height=7x18=126 pixels
      i3LcdString({                                            0,   0}, temperatureMax, LCD_COLOR_WHITE,  1);
      i3LcdString({                                            0, 119}, temperatureMin, LCD_COLOR_WHITE,  1); //126-7=119

      //i3LcdLineH({  0,148}, {319,148}, LCD_COLOR_GRAY); //(169-126)=43/2=169-21=148
      //i3LcdLineV({120,148}, {120,239},  LCD_COLOR_GRAY);

      i3LcdString({               (uint16_t)(319-pressureLength), 169},       pressure,   LCD_COLOR_OIL, 10); //Total height=7x10=70 pixels | 239-70=169
      i3LcdString({(uint16_t)(319-pressureLength-5-minMaxLength), 169},    pressureMax,   LCD_COLOR_OIL,  1);
      i3LcdString({(uint16_t)(319-pressureLength-5-minMaxLength), 232},    pressureMin,   LCD_COLOR_OIL,  1); //(169+70)-7=232

      i3LcdSwap();
    }
    vTaskDelay (100 / portTICK_PERIOD_MS);
  }
}
