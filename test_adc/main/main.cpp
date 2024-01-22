#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include <i3-battery.h>

#define TAG "I3-MAIN"

adc_oneshot_unit_handle_t adcHandle;
int value;

/**
 * Mesure de la tension de la batterie via un pont diviseur de tension
 * R1=517k R2=300k
 * Max : 3080mV (8.4V réel) -> Valeur adc : 3128
 * Min : 2280mV (6.2V réel) -> Valeur adc : 2346
 */
int batteryGetPercent(int batteryVoltage){
  return i3BatteryGetPercent(batteryVoltage, 2346, 3128);
}

/**
 * Return "3128mV (100%)"
 */
/*
char* getBattery(){
  static char ret[20] = "";
  int batteryVoltage = analogReadMilliVolts(ADC1_CH5);
  sprintf(ret, "%dmV (%d%%)",
    batteryVoltage,
    batteryGetPercent(batteryVoltage)
  );
  return ret;
}*/

void setup(){
  ESP_LOGV(TAG, "setup()");
  adc_oneshot_unit_init_cfg_t initConfig = {
    .unit_id = ADC_UNIT_2,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  esp_err_t err = adc_oneshot_new_unit(&initConfig, &adcHandle);
  if (err != ESP_OK) ESP_LOGE(TAG, "adc_oneshot_new_unit() failed");

  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
  };  
  err = adc_oneshot_config_channel(adcHandle, ADC_CHANNEL_5, &config);
  if (err != ESP_OK) ESP_LOGE(TAG, "adc_oneshot_config_channel() failed");
}

void read(){
  ESP_LOGI(TAG, "read()");
  esp_err_t err = adc_oneshot_read(adcHandle, ADC_CHANNEL_5, &value);
  if (err != ESP_OK) ESP_LOGE(TAG, "adc_oneshot_read() failed");
  ESP_LOGI(TAG, "Result: %d", value);
}

extern "C" void app_main(){
  //Log level
  esp_log_level_set(TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  setup();

  for(;;){
    //ESP_LOGI(TAG, "Battery: %s", getBattery());

    read();

    vTaskDelay (3000 / portTICK_PERIOD_MS);
  }
}
