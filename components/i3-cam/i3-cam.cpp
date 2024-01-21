#include "i3-cam.h"

/**
 * Take a screenshot
 */
camera_fb_t *i3CamScreenshot(camera_fb_t *camera_fb){
  camera_fb_t *ret;

  if (camera_fb) esp_camera_fb_return(camera_fb); //free memory
  ret = esp_camera_fb_get();
  ESP_LOGD(I3_CAM_TAG, "Length %d bytes", ret->len);
  return ret;
}

/**
 * Configuration
 */
camera_config_t i3CamConfiguration(){
  camera_config_t config;

  //Pins (AI Thinker - ESP32-CAM)
  config.pin_d0       = 5;
  config.pin_d1       = 18;
  config.pin_d2       = 19;
  config.pin_d3       = 21;
  config.pin_d4       = 36;
  config.pin_d5       = 39;
  config.pin_d6       = 34;
  config.pin_d7       = 35;
  config.pin_xclk     = 0;
  config.pin_pclk     = 22;
  config.pin_vsync    = 25;
  config.pin_href     = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn     = 32;
  config.pin_reset    = -1;

  config.fb_count = 1; //Number of frame buffers to be allocated. If more than one, then each frame will be acquired (double speed)

  //CAMERA_GRAB_WHEN_EMPTY Fills buffers when they are empty. Less resources but first 'fb_count' frames might be old
  //CAMERA_GRAB_LATEST     Except when 1 frame buffer is used, queue will always contain the last 'fb_count' frames
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  //CAMERA_FB_IN_PSRAM Frame buffer is placed in external PSRAM
  //CAMERA_FB_IN_DRAM  Frame buffer is placed in internal DRAM <= plante
  config.fb_location = CAMERA_FB_IN_PSRAM;

  config.ledc_channel = LEDC_CHANNEL_0; //?
  config.ledc_timer   = LEDC_TIMER_0;     //?
  config.xclk_freq_hz = 20000000;       //?

  //PIXFORMAT_JPEG : For streaming
  //PIXFORMAT_RGB565 : For face detection/recognition
  config.pixel_format = PIXFORMAT_JPEG;  //YUV422|GRAYSCALE

  //OV2640 (2MP)
  //De 00 (96x96) à 13 (1600x1200), il existe de multitude de résolution intermédiaire
  //La résolutio native du capteur étant du 1632x1232, il encaisse donc le UXGA (1600x1200)
  config.frame_size = FRAMESIZE_UXGA; //13

  //OV3660 (3MP)
  //FRAMESIZE_QXGA (2048x1536)

  //OV5640 (5MP)
  //FRAMESIZE_QSXGA (2560x1920)

  //La quality a un impact sur la mémoire/performance de l'ESP
  //En UXGA, je ne peux pas descendre en dessous de :
  // 9 avec CameraWebServer
  // 5 avec test_cam
  config.jpeg_quality = 9;  //0-63 plus bas = meilleure qualité

  return config;
}

/**
 * Init
 */
void i3CamInit(){
  ESP_LOGI(I3_CAM_TAG, "Initializing camera...");
  camera_config_t config = i3CamConfiguration();
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK){
    ESP_LOGE(I3_CAM_TAG, "When initializing the camera");
  }
  ESP_LOGI(I3_CAM_TAG, "Initializing camera... OK");
}
