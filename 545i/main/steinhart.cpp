#include "steinhart.h"

/**
 * Not very accurate
 */
float toVoltage(unsigned short AdcValue){
  return (AdcValue * 3.3) / 4095;
}

/**
 * °K -> °C
 */
float toCelcius(unsigned short Kelvin){
    return Kelvin - 273.15;
}

/**
 * Pont diviseur de tension
 */
float computeR(float Vout){
    return R1 * (Vout / (Vcc - Vout));
}

/**
 * Version de l'équation de Steinhart-Hart à 3 termes
 */
float computeTemperature(float R){
    float kelvin = 1 / (A + B * log(R) + C * pow(log(R), 3));
    return toCelcius(kelvin);
}
