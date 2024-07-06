#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <i3-ble.h>
#include <i3-adc.h>
#include <i3-mem.h>
#include <i3-table.h>
#include <i3-queue.h>

#define TAG "I3-MAIN"

#define GAP_SIZE 14
uint8_t adv_raw_data[GAP_SIZE] = {
  0x02,0x01,0x06,                //AD1: Flags
  0x04,0x09,0x4D,0x58,0x35,      //AD2: Complete Local Name "MX5"
  0x05,0x16,0x09,0x18,0x00,0x00, //AD3: Service Data 16-bit UUID 1809 (Health Thermometer Service)
  //0x05,0x16,0x6D,0x2A,0x14,0x00, //AD4: Service Data 16-bit UUID 2A6D (Pressure characteristic)
};

I3Queue* temperatureQueue = new I3Queue(10);

/**
 * Bosch temperature sensor
 */
#define TABLE_SIZE 19
int table[TABLE_SIZE][2] = {
  { 147, 140}, //0
  { 185, 130},
  { 234, 120},
  { 300, 110},
  { 370, 100},
  { 464,  90}, //5
  { 600,  80},
  { 776,  70},
  { 988,  60},
  {1258,  50},
  {1592,  40}, //10
  {1966,  30},
  {2366,  20},
  {2768,  10},
  {3153,   0},
  {3497, -10}, //15
  {3818, -20},
  {4084, -30},
  {4095, -40}, //18
};

/**
 * Temperature reading
 */
float getTemperature(){
    unsigned short value = i3AdcRead(ADC_CHANNEL_5);
    temperatureQueue->push(value);
    float average = temperatureQueue->average();
    float ret = i3TableGetValue(table, TABLE_SIZE, (int)average);
    ESP_LOGI(TAG, "adc value: %d average: %.2f temperature: %.2f", value, average, ret);   
    return ret;
}

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
  i3BleStartAdvertising(1000, adv_raw_data, GAP_SIZE);

  for(;;){
    short temperature = (short)(getTemperature()*100); //BLE want 7599 for 75.99°C
    i3CopyShort(adv_raw_data, 12, &temperature); //copie 2 octets à la position 12 (AD3)

    i3BleUpdateAdvertising(adv_raw_data, GAP_SIZE);

    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
