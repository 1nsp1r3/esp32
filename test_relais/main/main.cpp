#include <string>
#include <i3-string.h>
#include <i3-wifi.h>
#include <i3-httpd.h>
#include <driver/gpio.h>
#include "html.h"

#define TAG "I3-MAIN"

using namespace std;

string portailMode = "close";

string getIndex(){
  return i3StringReplace(index_html, "{portailMode}", portailMode);
}

void postAction(string data){
  ESP_LOGD(TAG, "Data recues: %s", data.c_str());
  list<string> pair = i3StringSplit(data, '=');
  portailMode = pair.back();

  if (portailMode == "open"){
    ESP_LOGI(TAG, "Ouverture...");
  }else if (portailMode == "close"){
    ESP_LOGI(TAG, "Fermeture...");
  }else{
    ESP_LOGE(TAG, "Unknown command");
  }
}

extern "C" void app_main(){
  //Log level
  esp_log_level_set(I3_WIFI_TAG , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_HTTPD_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(TAG         , ESP_LOG_VERBOSE);

  i3WifiInit();

  i3HttpdStart();
  i3HttpdAddGetEndpoint("/"       , getIndex);
  i3HttpdAddPostEndpoint("/action", postAction);

  gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);

  ESP_LOGI(TAG, "Loop");

  /*
  int i=0;
  for(;;){
    i++;
    if (i%2 == 0){
      gpio_set_level(GPIO_NUM_32, 1);
      ESP_LOGI(TAG, "Ouverture...");
    }else{
      gpio_set_level(GPIO_NUM_32, 0);
      ESP_LOGI(TAG, "Fermeture...");
    }

    vTaskDelay (2000 / portTICK_PERIOD_MS);
  }
  */
}
