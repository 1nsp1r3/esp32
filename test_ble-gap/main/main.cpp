#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "I3-MAIN"

int value;
int battery;

esp_ble_adv_params_t params = {
  .adv_int_min       = 1600, //1600 x 0.625 = 1000ms
  .adv_int_max       = 1600, //1600 x 0.625 = 1000ms
  .adv_type          = ADV_TYPE_NONCONN_IND,
  .own_addr_type     = BLE_ADDR_TYPE_PUBLIC,
  .channel_map       = ADV_CHNL_ALL,
  .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

//iBeacon
/*
uint8_t adv_raw_data[30] = {
// Len Type Value
  0x02,0x01,0x06,
  0x1A,0xFF,0x4C,0x00, //Company: Apple, Inc.
            0x02,      //Type: Beacon
            0x15,      //?
            0xFE,0xA5,0x06,0x93,0xA4,0xE2,0x4F,0xB1,0xAF,0xCF,0xC6,0xEB,0x07,0x64,0x78,0x25, //UUID
            0x00,0x00,0x00,0x00,0xC5 //?
};*/

uint8_t adv_raw_data[14] = {
// Len Type Value
  0x02,0x01,0x06,
  0x05,0x16,0x09,0x18,0x12,0x32, //1809 (Health thermometer) 17°
  0x04,0x09,0x52,0x58,0x38 //RX8
};

/*
uint8_t adv_raw_data[30] = {
// Len Type Value
  0x02,0x01,0x06,
  0x05,0x16,0x09,0x18,0x11,0x00, //1809 (Health thermometer) 17°
  0x04,0x09,0x4D,0x58,0x35, //MX5
  0x11,0x07, //?
  0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E //128-bit Service UUID
};*/

void initNvs(){
  ESP_LOGV(TAG, "initNvs()");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
    nvs_flash_erase();
    nvs_flash_init();
  }
}

void initBtController(){
  ESP_LOGV(TAG, "initBtController()");
  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  esp_bt_controller_init(&bt_cfg);
}

void enableBLE(){
  ESP_LOGV(TAG, "enableBLE()");
  esp_bt_controller_enable(ESP_BT_MODE_BLE);
}

void initBluedroid(){
  ESP_LOGV(TAG, "initBluedroid()");
  esp_bluedroid_init();
  esp_bluedroid_enable();
}

void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
      ESP_LOGV(TAG, "<event> ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT");
      esp_ble_gap_start_advertising(&params);
      break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
      ESP_LOGV(TAG, "<event> ESP_GAP_BLE_ADV_START_COMPLETE_EVT");
      if(param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS){
        ESP_LOGI(TAG, "Advertising started");
      }else{
        ESP_LOGE(TAG, "Unable to start advertising process");
      }
      break;
    default:
      ESP_LOGV(TAG, "<event> %d unhandled", event);
      break;
  }
}

/**
 *
 */
extern "C" void app_main(){
  esp_log_level_set(TAG, ESP_LOG_VERBOSE); //Log level

  ESP_LOGI(TAG, "Bonjour :-)");

  initNvs();
  initBtController();
  enableBLE();
  initBluedroid();

  esp_err_t ret = esp_ble_gap_register_callback(&esp_gap_cb);
  if (ret != ESP_OK){
    ESP_LOGE(TAG, "When esp_ble_gap_register_callback()");
    return;
  }
  ESP_LOGV(TAG, "esp_ble_gap_register_callback()");

  esp_ble_gap_config_adv_data_raw(adv_raw_data, 14);
  if (ret != ESP_OK){
    ESP_LOGE(TAG, "When esp_ble_gap_config_adv_data_raw()");
    return;
  }
  ESP_LOGV(TAG, "esp_ble_gap_config_adv_data_raw()");

  for(;;){
    //ESP_LOGI(TAG, "Hu?");
    vTaskDelay (1000 / portTICK_PERIOD_MS);
  }
}
