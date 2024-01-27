#ifndef I3_ADC_H
#define I3_ADC_H

#define I3_ADC_TAG "I3-ADC"

#include <esp_log.h>
#include <esp_adc/adc_oneshot.h>

static adc_oneshot_unit_handle_t adcHandle;
static int i3AdcValue;

void i3AdcInit(adc_unit_t Unit, adc_channel_t Channel);
int i3AdcRead(adc_channel_t Channel);
int i3AdcGetPercent(int AdcValue, int Min, int Max);

#endif
