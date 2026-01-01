#include <math.h>

#define A 0.001199520237
#define B 0.0001505791472
#define C 0.0000003956058234
#define Vcc 3.34 //V
#define R1 15020 //Ohms

float toVoltage(unsigned short AdcValue);
float toCelcius(unsigned short Kelvin);
float computeR(float Vout);
float computeTemperature(float R);
