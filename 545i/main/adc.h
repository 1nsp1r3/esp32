#include <i3-adc.h>
#include <i3-steinhart.h>
#include <i3-queue.h>

I3Queue *tempQueue = new I3Queue(5);
char temperature[7] = "---";
char tempMin[7]     = "---";
char tempMax[7]     = "---";
int16_t min = 200;
int16_t max = -200;


/**
 *
 */
void adcInit(){
  i3AdcInit(ADC_UNIT_1, ADC_CHANNEL_1); 
}

/**
 * 
 */
void adcProcess(){
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
