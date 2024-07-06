#include "i3-mem.h"

/**
 * Copie 2 octets
 */
void i3CopyShort(unsigned char* Destination, unsigned short Position, short* Value){
    ESP_LOGV(I3_MEM_TAG, "i3CopyShort()");
    memcpy(Destination+sizeof(char)*Position, Value, sizeof(short));
}

/**
 * Copie 4 octets
 */
void i3CopyFloat(unsigned char* Destination, unsigned short Position, float* Value){
    ESP_LOGV(I3_MEM_TAG, "i3CopyFloat()");
    memcpy(Destination+sizeof(char)*Position, Value, sizeof(float));
}
