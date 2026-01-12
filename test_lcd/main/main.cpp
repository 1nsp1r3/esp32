#include <time.h>
#include <i3-lcd.h>
#include <i3-lzw.h>
#include <i3-sys.h>
//#include "logo-mazda.h"
#include "mario.h"

#define TAG "I3-MAIN"

#define MAX_FRAMES 22
/**
 * Mario
 */
uint16_t frames[MAX_FRAMES][3] = {
  {  0, 0, false},
  { 32, 0, false},
  { 64, 0, false},
  { 96, 0, false},
  {128, 0, false},
  {160, 0, false},
  {192, 0, false},
  {224, 0, false},
  {256, 0, false},
  {288, 0, false},
  {320, 0, false},
  {352, 0, false},
  {320, 0, true},
  {288, 0, true},
  {256, 0, true},
  {224, 0, true},
  {192, 0, true},
  {160, 0, true},
  {128, 0, true},
  { 96, 0, true},
  { 64, 0, true},
  { 32, 0, true},
};

uint8_t currentFrame=0;

/**
 *
 */
void showNextFrame(uint8_t *sprites, uint16_t x, uint16_t y, uint8_t width, uint8_t height){
  i3LcdSprite(sprites, palette, frames[currentFrame][0], frames[currentFrame][1], IMAGE_WIDTH, x, y, width, height, frames[currentFrame][2]);
}

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  uint8_t* sprites = i3SysPsramMallocUint8(IMAGE_WIDTH*IMAGE_HEIGHT);
  i3Unzip(image, sizeof(image), sprites);

  i3LcdInit();
  for(;;){
    i3LcdClear(); 
    showNextFrame(sprites, 160, 120, 31, 30);
    i3LcdSwap();

    vTaskDelay (100 / portTICK_PERIOD_MS);

    currentFrame++;
    if (currentFrame >= MAX_FRAMES){
      currentFrame=0;
    }
  }
}
