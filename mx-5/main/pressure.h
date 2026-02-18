#include <i3-adc.h>
#include <i3-linear.h>

char pressure[7]    = "---";
char pressureMin[7] = "---";
char pressureMax[7] = "---";
float Pmin = 9.9;
float Pmax = 0.0;

/**
 *
 */
float psiToBars(float psiValue){
  return psiValue * 0.0689476;
}

/**
 * Pressure reading
 * O PSI -> 0.5V rabaissé à 0.37V soit 425 en lecture ADC
 * 145 PSI -> 4.5V rabaissé à 3.3V soit 4095 en lecture ADC
 */
void pressureProcess(){
  unsigned short adc = i3AdcRead(ADC_CHANNEL_3);
  float p = i3LinearGetX(0, 425, 145, 4095, adc);

  if (p < 0) p = 0;
  if (p >= 143) p = 143; //9.9bars

  float barsValue = psiToBars(p);
  if (barsValue > Pmax) Pmax = barsValue;
  if (barsValue < Pmin) Pmin = barsValue;

  sprintf(   pressure, "%.1f", barsValue);
  sprintf(pressureMin, "%.1f", Pmin);
  sprintf(pressureMax, "%.1f", Pmax);
}
