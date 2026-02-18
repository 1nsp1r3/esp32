#include "i3-linear.h"

float getSlope(int Xa, int Ya, int Xb, int Yb){
    float diffY = Yb - Ya;
    float diffX = Xb - Xa;
    return diffY / diffX;
}

/**
 * Calculate b (0 if the linear function cut the origin)
 */
float getB(int Xa, int Ya, float Slope){
    return !Xa && !Ya ? 0 : Ya - Slope * Xa;
}

float i3LinearGetY(int Xa, int Ya, int Xb, int Yb, int X){
    float slope = getSlope(Xa, Ya, Xb, Yb);
    float b = getB(Xa, Ya, slope);
    return X*slope + b;
}

float i3LinearGetX(int Xa, int Ya, int Xb, int Yb, int Y){
    float slope = getSlope(Xa, Ya, Xb, Yb);
    float b = getB(Xa, Ya, slope);
    return (Y-b) / slope;
}
