#ifndef I3_ANIMATION_H
  #define I3_ANIMATION_H

  #define I3_ANIMATION_TAG "I3-ANIMATION"

#include "Clip.h"

/**
 *
 */
class Animation {
  Clip* clipList[255];
  uint8_t clipPos = 0;
  bool isFinished = true;
  uint8_t length = 0;

  public:
    Animation(uint8_t length){
      this->clipPos = 0;
      this->length = length;
      this->isFinished = true;
    };

    void addClip(Clip *clip){
      this->clipList[this->clipPos++] = clip;
      if (this->clipPos >= this->length){
        this->clipPos = 0;
      }
    };

    void playFrame(){
      //ESP_LOGI(I3_ANIMATION_TAG, "playFrame()");
      if (this->isFinished){
        this->clipPos = 0;
        this->isFinished = false;
        for(uint8_t i=0;i<this->length;i++) this->clipList[i]->reset();
      }

      this->clipList[clipPos]->playFrame();

      if (this->clipList[clipPos]->isFinished){
        clipPos++;
        if (this->clipPos >= this->length){
          this->isFinished = true;
          //ESP_LOGI(I3_ANIMATION_TAG, "playFrame() isFinished=true");
        }
      }
    }
};

#endif