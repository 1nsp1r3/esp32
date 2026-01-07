#include <string.h>
#include <time.h>
#include <math.h>
#include <i3-lcd.h>
#include <i3-adc.h>
#include <i3-steinhart.h>
#include <i3-queue.h>
#include <i3-lzw.h>
#include "logo.h"

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

  I3Queue *tempQueue = new I3Queue(5);

  //-------------
  //LCD
  //-------------
  size_t length;
  unsigned char* splashscreen = i3Unzip(logo, sizeof(logo), length);

  i3LcdInit();
  i3LcdClear();
  i3LcdSprite(splashscreen, palette, 60, 20, 199, 199, 1);
  i3LcdSwap();

  vTaskDelay (5000 / portTICK_PERIOD_MS);

  //------
  //ADC
  //------
  char temperature[7] = "---";
  i3AdcInit(ADC_UNIT_1, ADC_CHANNEL_1); 

  time(&startTime);
  for(;;){
    processTime();

    i3LcdClear();
    i3LcdString(136, 208,       timer, LCD_COLOR_GREEN, 4);
    i3LcdString(2,     2, temperature, LCD_COLOR_WHITE, 20);
    i3LcdSwap();

    unsigned short adc = i3AdcRead(ADC_CHANNEL_1);
    int mV = i3AdcToVoltage(adc);
    float Vin = mV / (float)1000;
    float R = i3SteinhartGetR(Vin);
    float T = i3SteinhartGetTemperature(R);
    tempQueue->push(T);
    int avg = round(tempQueue->avg());

    //ESP_LOGI(TAG, "adc: %d Vin: %.2f R: %.0f T: %.0f avg: %d", adc, Vin, R, T, avg);

    sprintf(temperature, "%d", avg);

    vTaskDelay (2000 / portTICK_PERIOD_MS);
  }
}
