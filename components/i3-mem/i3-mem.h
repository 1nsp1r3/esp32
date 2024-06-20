#ifndef I3_MEM_H
#define I3_MEM_H

#define I3_MEM_TAG "I3-MEM"

#include <string.h>
#include <esp_log.h>

void i3CopyWord(unsigned char* destination, unsigned short position, unsigned short* data);

#endif
