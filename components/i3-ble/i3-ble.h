#ifndef I3_BLE_H
#define I3_BLE_H

#define I3_BLE_TAG "I3-BLE"

#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>

static esp_ble_adv_params_t gapParams;

void i3BleInit();
void i3BleStartAdvertising(ushort frequency, uint8_t* rawData, uint8_t size);
void i3BleUpdateAdvertising(uint8_t* rawData, uint8_t size);

#endif
