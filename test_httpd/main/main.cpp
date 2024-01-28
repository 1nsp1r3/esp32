#include <i3-string.h>
#include <i3-wifi.h>
#include <i3-httpd.h>
#include "html.h"

#define TAG "I3-MAIN"
string videoMode = "VGA";

string indexResponse; //Must be a global var (at least a local static var)

I3HttpdContent* content = new I3HttpdContent();

I3HttpdContent* getIndex(){
  indexResponse = i3StringReplace(index_html, "{videoMode}", videoMode);
  content->data = indexResponse.c_str();
  content->length = indexResponse.length();
  return content;
}

void postAction(const char* data){
  ESP_LOGD(TAG, "Data recues: %s", data);
  list<string> pair = i3StringSplit(string(data), '=');
  videoMode = pair.back();
}

extern "C" void app_main(){
  //Log level
  esp_log_level_set(I3_WIFI_TAG  , ESP_LOG_INFO);
  esp_log_level_set(I3_HTTPD_TAG , ESP_LOG_VERBOSE);
  //esp_log_level_set(I3_STRING_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(TAG          , ESP_LOG_VERBOSE);

  i3WifiInit();

  I3Httpd* i3Httpd = new I3Httpd();
  i3Httpd->start();
  i3Httpd->addGetEndpoint("/"       , getIndex);
  i3Httpd->addPostEndpoint("/action", postAction);

  strcpy(content->type, "text/html");
  ESP_LOGI(TAG, "Done");
}
