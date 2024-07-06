#ifndef I3_MEM_H
#define I3_MEM_H

#define I3_MEM_TAG "I3-MEM"

#include <string.h>
#include <esp_log.h>

void i3CopyShort(unsigned char* Destination, unsigned short Position, short* Value);
void i3CopyFloat(unsigned char* Destination, unsigned short Position, float* Value);

#endif
