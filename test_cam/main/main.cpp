#include <i3-wifi.h>
#include <i3-httpd.h>
#include <i3-cam.h>

#define TAG "I3-MAIN"

camera_fb_t *camera_fb = NULL;
GetReponse* getResponse = new GetReponse();

void screenshot(){
  ESP_LOGD(TAG, "Screenshot...");
  camera_fb = i3CamScreenshot(camera_fb);
  if (!camera_fb){
    ESP_LOGE(TAG, "When capturing image");
  }else{
    ESP_LOGD(TAG, "Screenshot... OK");
  }
}

GetReponse* getScreenshot(){
  screenshot();  
  getResponse->content = (const char *)camera_fb->buf;
  getResponse->contentLength = camera_fb->len;
  return getResponse;
}

extern "C" void app_main(){
  //Log level
  esp_log_level_set(I3_WIFI_TAG , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_HTTPD_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(I3_CAM_TAG  , ESP_LOG_VERBOSE);
  esp_log_level_set(TAG         , ESP_LOG_VERBOSE);

  i3WifiInit();
  i3CamInit();
  i3HttpdStart();
  i3HttpdAddGetEndpoint("/", getScreenshot);
  
  strcpy(getResponse->type, "image/jpeg");

  ESP_LOGI(TAG, "Done");
}
