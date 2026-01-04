#include "i3-steinhart.h"

/**
 * Pont diviseur de tension
 */
float i3SteinhartGetR(float Vin){
    return STEINHART_R1 * (Vin / (STEINHART_VOUT - Vin));
}

/**
 * Version de l'équation de Steinhart-Hart à 3 termes
 */
float i3SteinhartGetTemperature(float R){
    float kelvin = 1 / (STEINHART_A + STEINHART_B * log(R) + STEINHART_C * pow(log(R), 3));
    return STEINHART_2CELCIUS(kelvin);
}
