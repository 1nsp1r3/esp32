#include <i3-wifi.h>
#include <i3-httpd.h>
#include <i3-cam.h>

#define TAG "I3-MAIN"

camera_fb_t *camera_fb = NULL;
httpd_handle_t* server = NULL;

esp_err_t _handler(httpd_req_t *req){
  esp_err_t res = ESP_OK;
  char buffer_len[10];

  ESP_LOGD(TAG, "GET (%s)", req->uri);

  ESP_LOGD(TAG, "Screenshot...");
  camera_fb = i3CamScreenshot(camera_fb);
  if (!camera_fb){
    ESP_LOGE(TAG, "When capturing image");
    return ESP_FAIL;
  }
  ESP_LOGD(TAG, "Screenshot... OK");

  res = httpd_resp_set_type(req, "image/jpeg");
  if (res != ESP_OK){
    ESP_LOGE(TAG, "Unable to set content type");
    return res;
  }

  ESP_LOGD(TAG, "Try to send jpg data...");
  return httpd_resp_send(req, (const char *)camera_fb->buf, camera_fb->len);  
}

void _addEndPoint(){
  httpd_uri_t index_uri = {
    .uri       = "/test",
    .method    = HTTP_GET,
    .handler   = _handler,
    .user_ctx  = NULL
  };
  esp_err_t ret = httpd_register_uri_handler(*server, &index_uri);
  if (ret != ESP_OK){
    ESP_LOGE(TAG, "Unable to add get endpoint /test");
    return;
  }
  ESP_LOGI(TAG, "Get '/test' endpoint added");
}


extern "C" void app_main(){
  //Log level
  esp_log_level_set(I3_WIFI_TAG , ESP_LOG_VERBOSE);
  esp_log_level_set(I3_HTTPD_TAG, ESP_LOG_VERBOSE);
  esp_log_level_set(I3_CAM_TAG  , ESP_LOG_VERBOSE);
  esp_log_level_set(TAG         , ESP_LOG_VERBOSE);

  i3WifiInit();
  i3CamInit();
  server = i3HttpdStart();
  _addEndPoint();
  
  ESP_LOGI(TAG, "Done");
}
