#include "i3-sys.h"

/**
 *
 */
int i3SysGetSramSize(){
    return heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
}

/**
 *
 */
int i3SysGetSramAvailable(){
    return heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
}

/**
 *
 */
void i3SysDebugRam(){
    int total     = i3SysGetSramSize();
    int occupied  = total-i3SysGetSramAvailable();
    float percent = ((float)occupied / (float)total)*100;

    ESP_LOGI(I3_SYS_TAG, "RAM: %d/%d bytes (%.1f%%)", occupied, total, percent);
}

//-------------------------------------------------------------------------------------------------------
// PSAM (Must be enable in menuconfig and available with the chip)
//-------------------------------------------------------------------------------------------------------

/**
 *
 */
uint8_t* i3SysPsramMallocUint8(size_t size){
    return (uint8_t*)heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

/**
 *
 */
int i3SysGetPsramSize(){
    return heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
}

/**
 *
 */
int i3SysGetPsramAvailable(){
    return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
}

/**
 *
 */
void i3SysDebugPsram(){
    int total     = i3SysGetPsramSize();
    int occupied  = total-i3SysGetPsramAvailable();
    float percent = ((float)occupied / (float)total)*100;

    ESP_LOGI(I3_SYS_TAG, "PSRAM: %d/%d bytes (%.1f%%)", occupied, total, percent);
}
