#include "i3-mem.h"

/**
 * Copie 2 octets quelque part en mémoire
 */
void i3CopyWord(unsigned char* destination, unsigned short position, unsigned short* data){
    ESP_LOGV(I3_MEM_TAG, "i3CopyWord()");
    memcpy(destination+sizeof(char)*position, data, sizeof(short));
}

