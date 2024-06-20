#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <i3-adc.h>

#define TAG "I3-MAIN"

unsigned short value;
char battery;

/**
 *
 */
extern "C" void app_main(){
  //Log level
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_ADC_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  i3AdcInit(ADC_UNIT_2, ADC_CHANNEL_5);

  for(;;){
    value = i3AdcRead(ADC_CHANNEL_5);
    battery = i3AdcGetPercent(value, 2770, 4000);
    ESP_LOGI(TAG, "Result: %d (%d)", value, battery);

    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
