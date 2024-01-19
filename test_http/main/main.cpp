#include <i3-wifi.h>
#include <i3-http.h>

#define TAG "I3-MAIN"

extern "C" void app_main(){
  char datetime[20];
  I3Http* i3Http = new I3Http();

  //Init
  memset(datetime, 0, 20);

  //Log level
  esp_log_level_set(I3_WIFI_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(I3_HTTP_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(TAG        , ESP_LOG_VERBOSE);
  
  i3WifiInit();

  if (i3Http->get("http://worldtimeapi.org/api/timezone/Europe/Paris") == 200){
    ESP_LOGD(TAG, "length: %d", i3Http->length);
    ESP_LOGV(TAG, "%s", i3Http->body);

    strncpy(datetime, i3Http->body+62, 19);
    ESP_LOGI(TAG, "datetime: (%s)", datetime);
  }else{
    ESP_LOGE(TAG, "Query failed");  
  }

  delete i3Http;
}
