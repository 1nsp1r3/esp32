#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <i3-ble.h>
#include <i3-adc.h>
#include <i3-mem.h>
#include <i3-table.h>

#define TAG "I3-MAIN"

uint8_t adv_raw_data[20] = {
  0x02,0x01,0x06,                //AD1: Flags
  0x04,0x09,0x4D,0x58,0x35,      //AD2: Complete Local Name "MX5"
  0x05,0x16,0x09,0x18,0x12,0x00, //AD3: Service Data 16-bit UUID 1809 (Health Thermometer Service) 2 octets 1200 pour 17° <- Ca attend un float sur 16bits ici
  0x05,0x16,0x6D,0x2A,0x14,0x00, //AD4: Service Data 16-bit UUID 2A6D (Pressure characteristic)  
};

unsigned short value;
char battery;

int table[][2] = {
    {0, 140},
    {500, 110},
    {1000, 90},
    {1500, 70},
    {2000, 50},
    {2500, 30},
    {3000, 10},
    {4000, -10},
};

/**
 * MAIN
 */
extern "C" void app_main(){
  //Log level
  esp_log_level_set(TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(I3_BLE_TAG, ESP_LOG_INFO);

  ESP_LOGI(TAG, "Bonjour :-)");

  i3AdcInit(ADC_UNIT_2, ADC_CHANNEL_5);
  i3BleInit();
  i3BleStartAdvertising(1000, adv_raw_data, 26);

  float result = i3TableGetValue(table, 8, 2750);

  ESP_LOGI(TAG, "Result: %f", result);

  for(;;){
    value = i3AdcRead(ADC_CHANNEL_5);
    battery = i3AdcGetPercent(value, 2770, 4000);
    ESP_LOGI(TAG, "Result: %d (%d)", value, battery);

    //copie 2 octets de value à la position 12 (AD3)
    i3CopyWord(adv_raw_data, 12, &value);
    i3BleUpdateAdvertising(adv_raw_data, 26);

    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
