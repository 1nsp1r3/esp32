#include "i3-ble.h"

/**
 * Key/value memory used by BT controller and bluedroid
 */
void i3BleInitNvs(){
  ESP_LOGV(I3_BLE_TAG, "i3BleInitNvs()");
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
    nvs_flash_erase();
    nvs_flash_init();
  }
}

/**
 * Bluetooth controller
 */
void i3BleInitBtController(){
  ESP_LOGV(I3_BLE_TAG, "i3BleInitBtController()");
  esp_bt_controller_config_t config = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  i3BleInitNvs(); //NVS is used by BT controller and bluedroid
  esp_bt_controller_init(&config);
  esp_bt_controller_enable(ESP_BT_MODE_BLE);
}

/**
 * BLE stack like nimBLE
 */
void i3BleInitBluedroid(){
  ESP_LOGV(I3_BLE_TAG, "i3BleInitBluedroid()");
  esp_bluedroid_init();
  esp_bluedroid_enable();
}

/**
 * Handler
 */
void i3BleGapHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
  ESP_LOGV(I3_BLE_TAG, "i3BleGapHandler()");

  switch (event){
    case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
      ESP_LOGV(I3_BLE_TAG, "<event> ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT");
      esp_ble_gap_start_advertising(&gapParams);
      break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
      ESP_LOGV(I3_BLE_TAG, "<event> ESP_GAP_BLE_ADV_START_COMPLETE_EVT");
      if(param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS){
        ESP_LOGE(I3_BLE_TAG, "Unable to start advertising process");
      }
      break;
    default:
      ESP_LOGV(I3_BLE_TAG, "<event> %d unhandled", event);
      break;
  }
}

/**
 * Init
 */
void i3BleInit(){
  ESP_LOGV(I3_BLE_TAG, "i3BleInit()");
  i3BleInitBtController();
  i3BleInitBluedroid();
}

/**
 * Advertising raw data (GAP)
 * @param frequency (ms)
 */
void i3BleStartAdvertising(ushort frequency, uint8_t* rawData, uint8_t size){
  ESP_LOGV(I3_BLE_TAG, "i3BleStartAdvertising()");
  gapParams.adv_int_min       = (ushort)(frequency / 0.625);
  gapParams.adv_int_max       = (ushort)(frequency / 0.625);
  gapParams.adv_type          = ADV_TYPE_NONCONN_IND;
  gapParams.own_addr_type     = BLE_ADDR_TYPE_PUBLIC;
  gapParams.channel_map       = ADV_CHNL_ALL;
  gapParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;

  esp_ble_gap_register_callback(&i3BleGapHandler);
  esp_ble_gap_config_adv_data_raw(rawData, size);
}

/**
 * Update advertising raw data (GAP)
 */
void i3BleUpdateAdvertising(uint8_t* rawData, uint8_t size){
  ESP_LOGV(I3_BLE_TAG, "i3BleUpdateAdvertising()");
  esp_ble_gap_config_adv_data_raw(rawData, size);
}
