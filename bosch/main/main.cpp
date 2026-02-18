#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <i3-ble.h>
#include <i3-adc.h>
#include <i3-mem.h>
#include <i3-linear.h>
#include <i3-table.h>
#include <i3-queue.h>

#define TAG "I3-MAIN"

//#define WITH_QUEUE //Comment if you don't want an average value

#define GAP_SIZE 20
uint8_t adv_raw_data[GAP_SIZE] = {
  0x02,0x01,0x06,                //AD1: Flags
  0x04,0x09,0x4D,0x58,0x35,      //AD2: Complete Local Name "MX5"
  0x05,0x16,0x09,0x18,0x00,0x00, //AD3: Service Data 16-bit UUID 1809 (Health Thermometer Service) SHORT
  0x05,0x16,0x6D,0x2A,0x00,0x00, //AD4: Service Data 16-bit UUID 2A6D (Pressure characteristic) SHORT
};

#ifdef WITH_QUEUE
  I3Queue* temperatureQueue = new I3Queue(10);
  I3Queue* pressureQueue = new I3Queue(10);
#endif

/**
 * Bosch temperature sensor avec R1=1774 Ohms
 */
#define TABLE_SIZE 19
int table[TABLE_SIZE][2] = {
  { 147, 140}, //0
  { 183, 130},
  { 233, 120},
  { 291, 110},
  { 363, 100},
  { 471,  90}, //5
  { 605,  80},
  { 771,  70},
  { 989,  60},
  {1268,  50},
  {1594,  40}, //10
  {1968,  30},
  {2352,  20},
  {2752,  10},
  {3124,   0},
  {3499, -10}, //15
  {3820, -20},
  {4095, -30},
  {4095, -40}, //18
};

/**
 * Temperature reading
 */
float getTemperature(){
    unsigned short adcValue = i3AdcRead(ADC_CHANNEL_5);

    #ifdef WITH_QUEUE
      temperatureQueue->push(adcValue);
      float temperature = i3TableGetValue(table, TABLE_SIZE, (int)temperatureQueue->average());
    #else
      float temperature = i3TableGetValue(table, TABLE_SIZE, adcValue);
    #endif

    ESP_LOGI(TAG, "adc value: %d temperature: %.2f °C", adcValue, temperature);
    return temperature;
}

/**
 * Pressure reading
 * O PSI -> 0.5V rabaissé à 0.37V soit 425 en lecture ADC
 * 145 PSI -> 4.5V rabaissé à 3.3V soit 4095 en lecture ADC
 */
float getPressure(){
    unsigned short adcValue = i3AdcRead(ADC_CHANNEL_6);

    #ifdef WITH_QUEUE
      pressureQueue->push(adcValue);
      float pressure = i3LinearGetX(0, 425, 145, 4095, (int)pressureQueue->average());
    #else
      float pressure = i3LinearGetX(0, 425, 145, 4095, adcValue);
    #endif

    if (pressure < 0) pressure = 0;
    ESP_LOGI(TAG, "adc value: %d pressure: %.2f PSI", adcValue, pressure);
    return pressure;
}

/**
 * MAIN
 */
extern "C" void app_main(){
  //Log level
  esp_log_level_set(TAG, ESP_LOG_NONE); //ESP_LOG_VERBOSE
  //esp_log_level_set(I3_BLE_TAG, ESP_LOG_INFO);

  ESP_LOGI(TAG, "Bonjour :-)");
  #ifdef WITH_QUEUE
    ESP_LOGI(TAG, "Queues enabled");
  #else
    ESP_LOGI(TAG, "Queues disabled");
  #endif

  i3AdcInit(ADC_UNIT_2, ADC_CHANNEL_5);
  i3AdcInit(ADC_UNIT_2, ADC_CHANNEL_6);
  i3BleInit();
  i3BleStartAdvertising(1000, adv_raw_data, GAP_SIZE);

  for(;;){
    short temperature = (short)(getTemperature()*100); //BLE want 7599 for 75.99°C
    i3CopyShort(adv_raw_data, 12, &temperature); //copie 2 octets à la position 12 (AD3)

    short pressure = (short)(getPressure()*100); //BLE want 7599 for 75.99 PSI
    i3CopyShort(adv_raw_data, 18, &pressure); //copie 2 octets à la position 12 (AD3)

    i3BleUpdateAdvertising(adv_raw_data, GAP_SIZE);

    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
