#include "i3-adc.h"

void i3AdcInit(adc_unit_t Unit, adc_channel_t Channel){
  ESP_LOGV(I3_ADC_TAG, "i3AdcInit(Unit: %d)", Unit);

  adc_oneshot_unit_init_cfg_t initConfig = {
    .unit_id = Unit,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  esp_err_t err = adc_oneshot_new_unit(&initConfig, &adcHandle);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_new_unit() failed");

  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  err = adc_oneshot_config_channel(adcHandle, Channel, &config);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_config_channel() failed");
}

int i3AdcRead(adc_channel_t Channel){
  ESP_LOGV(I3_ADC_TAG, "i3AdcRead()");
  esp_err_t err = adc_oneshot_read(adcHandle, Channel, &i3AdcValue);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_read() failed");
  return i3AdcValue;
}

/**
 * Usage : i3BatteryGetPercent(2695, 2270, 3120) -> 50
 * Marche aussi bien avec une valeur adc en millivolts ou entre 0 et 4095
 * 0.00V       0000
 * 0.50V       0550
 * 1.00V       1200
 * 1.50V       1810
 * 2.00V       2450
 * 2.34V       2850
 * 2.50V       3050
 * 3.00V       3860
 * 3.10-3.30V  4095
 */
int i3AdcGetPercent(int AdcValue, int Min, int Max) {
  ESP_LOGV(I3_ADC_TAG, "i3AdcGetPercent(AdcValue: %d, Min: %d, Max: %d)", AdcValue, Min, Max);

  //Check bounds
  int input = (AdcValue > Max) ? Max : AdcValue;
  input = (input < Min) ? Min : input;

  //Algo
  int diff = Max - Min; //3120mV - 2270mV = 850mV
  int normalize = input - Min; //E.g. 2695mV - 2270mV = 425mV

  int percent = (normalize*10) / (diff/10); //425mV / 850mV = 0.5 but with integer values : 4250 / 85 = 50
  return percent;
}
