#include <string.h>
#include <time.h>
#include <i3-lcd.h>
#include <i3-adc.h>
#include <i3-steinhart.h>
#include <i3-queue.h>

#define TAG "I3-MAIN"

time_t startTime;
char timer[9];
unsigned int numberOfSeconds = 0;

/**
 * Calculate 00:00:00
 */
void processTime(){
    time_t currentTime;
  
    time(&currentTime);
    numberOfSeconds = currentTime-startTime;

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

  ESP_LOGI(TAG, "Bonjour :-)");

  //------
  //LCD
  //------
  uint16_t *buffer = (uint16_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

  if (buffer != NULL) ESP_LOGI(TAG, "Initializing back buffer... OK");
  i3LcdInit();

  //------
  //ADC
  //------
  char temperature[7] = "---";
  int adcValue = 0;
  uint8_t adcCount = 0;

  i3AdcInit(ADC_UNIT_1, ADC_CHANNEL_1);
  
  I3Queue *vinQ = new I3Queue(10);
  I3Queue *rQ = new I3Queue(10);

  time(&startTime);
  for(;;){
    processTime();

    i3LcdClear(buffer);    
    i3LcdString(buffer, 2, 0, timer, LCD_COLOR_GREEN, 4);
    i3LcdString(buffer, 10, 100, temperature, LCD_COLOR_WHITE, 10);
    i3LcdString(buffer, 2, 305, "Inspir3 (c) 2026", LCD_COLOR_BLUE, 1);

    i3LcdSwap(buffer);

    unsigned short adc = i3AdcRead(ADC_CHANNEL_1);
    int mV = i3AdcToVoltage(adc);

    float Vin = mV / (float)1000;
    float R = i3SteinhartGetR(Vin);

    vinQ->push(Vin);
    rQ->push(R);

    if (vinQ->length() >= 10){
      ESP_LOGI(TAG, "Vin min: %.2f max: %.2f avg: %.2f | R min: %.2f max: %.2f avg: %.2f"
        , vinQ->min(), vinQ->max(), vinQ->avg()
        , rQ->min(), rQ->max(), rQ->avg());

      vinQ->clear();
      rQ->clear();
    }

    //float T = i3SteinhartGetTemperature(R);
    //sprintf(temperature, "%d", (short)round(T));

    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
