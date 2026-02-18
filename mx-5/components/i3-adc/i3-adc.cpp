#include "i3-adc.h"

void i3AdcInit(adc_unit_t Unit){
  ESP_LOGV(I3_ADC_TAG, "i3AdcInit()");

  adc_oneshot_unit_init_cfg_t initConfig = {
    .unit_id = Unit,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  esp_err_t err = adc_oneshot_new_unit(&initConfig, &adcHandle);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_new_unit() failed");

  adc_cali_curve_fitting_config_t cali_config = {
    .unit_id = Unit,
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  err = adc_cali_create_scheme_curve_fitting(&cali_config, &adcCaliHandle);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_cali_create_scheme_curve_fitting() failed");
}

void i3AdcInitChannel(adc_channel_t Channel){
  ESP_LOGV(I3_ADC_TAG, "i3AdcInitChannel()");

  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  esp_err_t err = adc_oneshot_config_channel(adcHandle, Channel, &config);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_config_channel() failed");
}

unsigned short i3AdcRead(adc_channel_t Channel){
  ESP_LOGV(I3_ADC_TAG, "i3AdcRead()");

  int adcValue;

  esp_err_t err = adc_oneshot_read(adcHandle, Channel, &adcValue);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_oneshot_read() failed");
  return (unsigned short)adcValue;
}

/**
 * @return int millivolts
 */
unsigned short i3AdcToVoltage(unsigned short AdcValue){
  ESP_LOGV(I3_ADC_TAG, "i3AdcToVoltage()");

  int voltageMv;  // En millivolts

  esp_err_t err = adc_cali_raw_to_voltage(adcCaliHandle, AdcValue, &voltageMv);
  if (err != ESP_OK) ESP_LOGE(I3_ADC_TAG, "adc_cali_raw_to_voltage() failed");

  return (unsigned short)voltageMv;
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
char i3AdcGetPercent(unsigned short AdcValue, unsigned short Min, unsigned short Max) {
  ESP_LOGV(I3_ADC_TAG, "i3AdcGetPercent(AdcValue: %d, Min: %d, Max: %d)", AdcValue, Min, Max);

  //Check bounds
  unsigned short input = (AdcValue > Max) ? Max : AdcValue;
  input = (input < Min) ? Min : input;

  //Algo
  unsigned short diff = Max - Min; //3120mV - 2270mV = 850mV
  unsigned short normalize = input - Min; //E.g. 2695mV - 2270mV = 425mV

  unsigned short percent = (normalize*10) / (diff/10); //425mV / 850mV = 0.5 but with integer values : 4250 / 85 = 50
  return (char)percent;
}
