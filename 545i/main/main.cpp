#include <string.h>
#include <time.h>
#include <math.h>
#include <i3-lcd.h>
#include <i3-adc.h>
#include <i3-steinhart.h>
#include <i3-queue.h>
#include <i3-lzw.h>
#include "logo-bmw.h"

#define TAG "I3-MAIN"

time_t startTime;
char timer[9];
 
/**
 * Generate a timer "00:00:00"
 */
void processTime(){
    time_t currentTime;
  
    time(&currentTime);
    unsigned int numberOfSeconds = currentTime-startTime;

    unsigned int hours   = numberOfSeconds / 3600;
    unsigned int minutes = (numberOfSeconds % 3600) / 60;
    unsigned int seconds = numberOfSeconds % 60;

    if (hours > 99) hours = 99;
    if (minutes > 59) minutes = 59;
    if (seconds > 59) seconds = 59;

    snprintf(timer, sizeof(timer), "%02u:%02u:%02u", hours, minutes, seconds);
}

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  //-------------
  //LCD
  //-------------
    unsigned char* splashscreen = i3Unzip(logo, sizeof(logo), LOGO_WIDTH*LOGO_HEIGHT);

  i3LcdInit();
  i3LcdClear();
  i3LcdSprite(splashscreen, palette, 60, 20, LOGO_WIDTH, LOGO_HEIGHT);
  i3LcdSwap();

  vTaskDelay (5000 / portTICK_PERIOD_MS);

  //------
  //ADC
  //------
  I3Queue *tempQueue = new I3Queue(5);
  char temperature[7] = "---";
  char tempMin[7]     = "---";
  char tempMax[7]     = "---";
  int16_t min = 200;
  int16_t max = -200;
  i3AdcInit(ADC_UNIT_1, ADC_CHANNEL_1); 

  time(&startTime);

  uint16_t tempLength   = i3LcdLength("---", 20);
  uint16_t timerLength  = i3LcdLength("00:00:00", 4);
  uint16_t minMaxLength = i3LcdLength("---", 1);

  uint16_t count = 0;
  for(;;){
    processTime();

    i3LcdClear();
    i3LcdString(160-(timerLength/2), 209,       timer, LCD_COLOR_GREEN, 4);  //209 = 239-(7x4)-2
    i3LcdString(160-(tempLength/2),   35, temperature, LCD_COLOR_WHITE, 20); //35 = (210/2)-((7x20)/2)
    i3LcdString(2,                   232,     tempMin, LCD_COLOR_WHITE, 1);
    i3LcdString(318-minMaxLength,    232,     tempMax, LCD_COLOR_WHITE, 1);
    i3LcdSwap();

    vTaskDelay (1000 / portTICK_PERIOD_MS);
    count++;

    if (count % 2 == 0){ //Every 2s, read adc
      unsigned short adc = i3AdcRead(ADC_CHANNEL_1);
      int mV = i3AdcToVoltage(adc);
      float Vin = mV / (float)1000;
      float R = i3SteinhartGetR(Vin);
      float T = i3SteinhartGetTemperature(R);
      tempQueue->push(T);
      
      int16_t avg = round(tempQueue->avg());
      if (avg > max) max = avg;
      if (avg < min) min = avg;

      sprintf(temperature, "%d", avg);
      sprintf(tempMin, "%d", min);
      sprintf(tempMax, "%d", max);
    }
  }
}
