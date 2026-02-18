#include <i3-adc.h>
#include <i3-steinhart.h>
#include <i3-queue.h>

I3Queue *temperatureQueue = new I3Queue(5);
char temperature[7]    = "---";
char temperatureMin[7] = "---";
char temperatureMax[7] = "---";
int16_t Tmin = 200;
int16_t Tmax = -200;

/**
 *
 */
void temperatureProcess(){
  unsigned short adc = i3AdcRead(ADC_CHANNEL_1);
  int mV = i3AdcToVoltage(adc);
  float Vin = mV / (float)1000;
  float R = i3SteinhartGetR(Vin);
  float T = i3SteinhartGetTemperature(R);
  temperatureQueue->push(T);

  int16_t avg = round(temperatureQueue->avg());
  if (avg > Tmax) Tmax = avg;
  if (avg < Tmin) Tmin = avg;

  sprintf(temperature, "%d", avg);
  sprintf(temperatureMin, "%d", Tmin);
  sprintf(temperatureMax, "%d", Tmax);
}
