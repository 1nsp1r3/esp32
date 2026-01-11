#ifndef I3_SYS_H
#define I3_SYS_H

#define I3_SYS_TAG "I3-SYS"

#include <esp_heap_caps.h>
#include <esp_log.h>

int i3SysGetSramSize();
int i3SysGetSramAvailable();
void i3SysDebugRam();

uint8_t* i3SysPsramMallocUint8(size_t size);
int i3SysGetPsramSize();
int i3SysGetPsramAvailable();
void i3SysDebugPsram();

#endif
