#ifndef I3_WIFI_H
#define I3_WIFI_H

#define I3_WIFI_TAG "I3-WIFI"

#include <string.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_wifi.h>

void i3WifiInit(const char* ssid = NULL, const char* password = NULL);

#endif
