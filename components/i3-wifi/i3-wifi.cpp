#include "i3-wifi.h"

/**
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html
 * https://github.com/espressif/esp-idf/blob/b3f7e2c8a4d354df8ef8558ea7caddc07283a57b/examples/wifi/wps/main/wps.c
 * https://github.com/espressif/esp-idf/blob/b3f7e2c8a4d354df8ef8558ea7caddc07283a57b/examples/wifi/antenna/main/antenna_switch_example_main.c
 * https://techoverflow.net/?s=wifi
 */
bool connected = false;
bool ipReceived = false;

/**
 * Handler
 */
void i3WifiEventHandler(void* Arg, esp_event_base_t Event_base, int32_t Event_id, void* Event_data){
  ESP_LOGV(I3_WIFI_TAG, "i3WifiEventHandler()");

  if (Event_base == WIFI_EVENT){
    if (Event_id == WIFI_EVENT_STA_START){
      ESP_LOGV(I3_WIFI_TAG, "WIFI_EVENT/WIFI_EVENT_STA_START");
      esp_wifi_connect();
      connected = true;
    }
    if (Event_id == WIFI_EVENT_STA_DISCONNECTED) ESP_LOGV(I3_WIFI_TAG, "WIFI_EVENT/WIFI_EVENT_STA_DISCONNECTED");
  }//WIFI_EVENT

  if (Event_base == IP_EVENT){
    if (Event_id == IP_EVENT_STA_GOT_IP){
      ESP_LOGV(I3_WIFI_TAG, "IP_EVENT/IP_EVENT_STA_GOT_IP");
      ip_event_got_ip_t* event = (ip_event_got_ip_t*)Event_data;
      ESP_LOGD(I3_WIFI_TAG, "IP: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
      ipReceived = true;
    }
  }//IP_EVENT
}

/**
 * Requirements for Wifi connection
 */
void i3WifiCheckRequirement(){
  ESP_LOGV(I3_WIFI_TAG, "i3WifiCheckRequirement()");
  ESP_LOGI(I3_WIFI_TAG, "Check requirements...");

  //NVS is used to store PHY calibration data
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
    nvs_flash_erase();
    nvs_flash_init();
  }

  esp_netif_init(); //Initialize TCP/IP network interface (required for Wi-Fi)
  esp_event_loop_create_default(); //Initialize the event loop
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_mode(WIFI_MODE_STA);

  esp_event_handler_instance_register(
    WIFI_EVENT,
    ESP_EVENT_ANY_ID,
    &i3WifiEventHandler,
    NULL,
    NULL
  );
  esp_event_handler_instance_register(
    IP_EVENT,
    IP_EVENT_STA_GOT_IP,
    &i3WifiEventHandler,
    NULL,
    NULL
  );
  ESP_LOGI(I3_WIFI_TAG, "Check requirements... OK");
}


/**
 *
 */
void i3WifiInit(const char *Ssid, const char *Password){
  ESP_LOGV(I3_WIFI_TAG, "i3WifiInit(Ssid: '%s', Password: '%s')", Ssid == NULL ? "" : Ssid, Password == NULL ? "" : Password);

  i3WifiCheckRequirement();
  ESP_LOGI(I3_WIFI_TAG, "Connecting...");

  wifi_config_t wifi_config = {
      .sta = {
          .ssid = "Livebox-23C0",
          .password = "sdrXVUrXthGwRxZnAH",
      }
  };

  if (Ssid != NULL)     strcpy ((char*)wifi_config.sta.ssid, Ssid);
  if (Password != NULL) strcpy ((char*)wifi_config.sta.password, Password);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

  esp_wifi_start();
  while(!connected || !ipReceived){ //Wait for connection
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(I3_WIFI_TAG, "Connecting... OK");
}
