#ifndef I3_TABLE_H
#define I3_TABLE_H

#include "i3-linear.h"

int i3TableIndexOfPreviousValue(int Table[][2], int Size, int Value);
int i3TableIndexOfNextValue(int Table[][2], int Size, int Value);
float i3TableGetValue(int Table[][2], int Size, int Value);

#endif
