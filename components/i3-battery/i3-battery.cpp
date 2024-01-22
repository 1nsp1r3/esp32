#include "i3-battery.h"

#include "i3-battery.h"

/**
 * Usage : getPercent(2695, 2270, 3120) -> 50
 */
int i3BatteryGetPercent(int analogReadMilliVolts, int min, int max) {
  //Check bounds
  int input = (analogReadMilliVolts > max) ? max : analogReadMilliVolts;
  input = (input < min) ? min : input;

  //Algo
  int diff = max - min; //3120mV - 2270mV = 850mV
  int normalize = input - min; //E.g. 2695mV - 2270mV = 425mV

  int percent = (normalize*10) / (diff/10); //425mV / 850mV = 0.5 but with integer values : 4250 / 85 = 50
  return percent;
}
