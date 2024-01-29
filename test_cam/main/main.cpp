#include <i3-wifi.h>
#include <i3-stream-server.h>
#include <i3-cam.h>

#define TAG "I3-MAIN"

camera_fb_t *camera_fb = NULL;
I3HttpdContent* content = new I3HttpdContent();

void screenshot(){
  ESP_LOGD(TAG, "Screenshot...");
  camera_fb = i3CamScreenshot(camera_fb);
  if (!camera_fb){
    ESP_LOGE(TAG, "When capturing image");
  }else{
    ESP_LOGD(TAG, "Screenshot... OK");
  }
}

I3HttpdContent* getScreenshot(){
  screenshot();
  content->data = (const char *)camera_fb->buf;
  content->length = camera_fb->len;
  return content;
}

extern "C" void app_main(){
  //Log level
  esp_log_level_set(I3_WIFI_TAG         , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_HTTPD_TAG        , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_STREAM_SERVER_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(I3_CAM_TAG          , ESP_LOG_VERBOSE);
  esp_log_level_set(TAG                 , ESP_LOG_VERBOSE);

  i3WifiInit();
  i3CamInit();

  I3StreamServer* i3StreamServer = new I3StreamServer();
  i3StreamServer->start();
  i3StreamServer->addGetEndpoint("/"         , getScreenshot);
  i3StreamServer->addStreamEndpoint("/stream", getScreenshot);

  strcpy(content->type, "image/jpeg");

  ESP_LOGI(TAG, "Done");
}
