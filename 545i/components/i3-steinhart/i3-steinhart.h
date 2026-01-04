#ifndef I3_STEINHART_H
#define I3_STEINHART_H

#include <math.h>

//A, B & C coefficients below are for the Tbest temperature sensor (545i)
//Use a calculator like this https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html
#define STEINHART_A    0.001199520237
#define STEINHART_B    0.0001505791472
#define STEINHART_C    0.0000003956058234

#define STEINHART_VOUT 3.3 //V (ESP32-S3-WAVESHARE 3V3 pin)
#define STEINHART_R1   15000 //Ohms (Pont diviseur de tension)

#define STEINHART_2CELCIUS(Kelvin) (Kelvin - 273.15) //°K -> °C

float i3SteinhartGetR(float Vin);
float i3SteinhartGetTemperature(float R);

#endif
