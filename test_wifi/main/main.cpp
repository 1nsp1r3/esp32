#include <i3-wifi.h>

extern "C" void app_main(){
  esp_log_level_set(I3_WIFI_TAG, ESP_LOG_VERBOSE);
  i3WifiInit();
}
