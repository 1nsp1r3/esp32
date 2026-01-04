#ifndef I3_ADC_H
#define I3_ADC_H

#define I3_ADC_TAG "I3-ADC"

#include <esp_log.h>
#include <esp_adc/adc_oneshot.h>

static adc_oneshot_unit_handle_t adcHandle = NULL;
static adc_cali_handle_t adcCaliHandle = NULL;

void i3AdcInit(adc_unit_t Unit, adc_channel_t Channel);
unsigned short i3AdcRead(adc_channel_t Channel);
unsigned short i3AdcToVoltage(unsigned short AdcValue);
char i3AdcGetPercent(unsigned short AdcValue, unsigned short Min, unsigned short Max);

#endif
