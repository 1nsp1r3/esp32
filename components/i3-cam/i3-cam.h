#ifndef I3_CAM_H
#define I3_CAM_H

#define I3_CAM_TAG "I3-CAM"

#include <esp_log.h>
#include <esp_camera.h>

void i3CamInit();
camera_fb_t *i3CamScreenshot(camera_fb_t *camera_fb);

#endif
