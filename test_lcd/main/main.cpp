#include <time.h>
#include <i3-lcd.h>
#include <i3-lzw.h>
#include <i3-sys.h>
//#include "logo-mazda.h"
#include "mario.h"

#include "Animation.h"
#include "TranslationClip.h"

#define TAG "I3-MAIN"

#define MAX_FRAMES 22
/**
 * Mario
 */
uint16_t frames[MAX_FRAMES][3] = {
  {  0, 0, false}, //0 Rear
  { 32, 0, false},
  { 64, 0, false},
  { 96, 0, false},
  {128, 0, false},
  {160, 0, false},
  {192, 0, false},
  {224, 0, false}, //7 Right
  {256, 0, false},
  {288, 0, false},
  {320, 0, false}, 
  {352, 0, false}, //11 Front
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
uint8_t* sprites;

/**
 *
 */
void showNextFrame(uint8_t *sprites, uint16_t x, uint16_t y, uint8_t width, uint8_t height){
  i3LcdSprite(sprites, palette, frames[currentFrame][0], frames[currentFrame][1], IMAGE_WIDTH, x, y, width, height, frames[currentFrame][2]);
}

void translationLeft2RightDisplay(uint16_t x, uint16_t y){
  i3LcdSprite(sprites, palette, frames[7][0], frames[7][1], IMAGE_WIDTH, x, y, 31, 30, false);
}

void translationTop2BottomDisplay(uint16_t x, uint16_t y){
  i3LcdSprite(sprites, palette, frames[11][0], frames[11][1], IMAGE_WIDTH, x, y, 31, 30, false);
}

void translationRight2LeftDisplay(uint16_t x, uint16_t y){
  i3LcdSprite(sprites, palette, frames[7][0], frames[7][1], IMAGE_WIDTH, x, y, 31, 30, true);
}

void translationBottom2TopDisplay(uint16_t x, uint16_t y){
  i3LcdSprite(sprites, palette, frames[0][0], frames[0][1], IMAGE_WIDTH, x, y, 31, 30, false);
}

Animation* avi;
TranslationClip translationLeft2Right("translationLeft2Right",      0,      0, 319-31,    240,  1,  0, &translationLeft2RightDisplay);
TranslationClip translationTop2Bottom("translationTop2Bottom", 319-31,      0,    320, 239-30,  0,  1, &translationTop2BottomDisplay);
TranslationClip translationRight2Left("translationRight2Left", 319-31, 239-30,      0,    240, -1,  0, &translationRight2LeftDisplay);
TranslationClip translationBottom2Top("translationBottom2Top",      0, 239-30,    320,      0,  0, -1, &translationBottom2TopDisplay);

/**
 * MAIN
 */
extern "C" void app_main(){
  esp_log_level_set(TAG       , ESP_LOG_VERBOSE); //Log level (ESP_LOG_NONE|ESP_LOG_VERBOSE)
  esp_log_level_set(I3_LCD_TAG, ESP_LOG_VERBOSE);

  ESP_LOGI(TAG, "Bonjour :-)");

  sprites = i3SysPsramMallocUint8(IMAGE_WIDTH*IMAGE_HEIGHT);
  i3Unzip(image, sizeof(image), sprites);

  avi = new Animation(4);
  avi->addClip(&translationLeft2Right);
  avi->addClip(&translationTop2Bottom);
  avi->addClip(&translationRight2Left);
  avi->addClip(&translationBottom2Top);

  i3LcdInit();
  for(;;){
    i3LcdClear(); 
    showNextFrame(sprites, 160, 120, 31, 30);
    avi->playFrame();
    i3LcdSwap();

    vTaskDelay (100 / portTICK_PERIOD_MS);

    currentFrame++;
    if (currentFrame >= MAX_FRAMES){
      currentFrame=0;
    }
  }
}
