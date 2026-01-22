#ifndef I3_TRANSLATION_CLIP_H
  #define I3_TRANSLATION_CLIP_H

  #define I3_TRANSLATION_CLIP_TAG "I3-TRANSLATION-CLIP"

#include "Clip.h"

using Operation = void (*)(uint16_t, uint16_t);

/**
 *
 */
class TranslationClip : public Clip{
  public:
    uint16_t x;
    uint16_t y;
    uint16_t startX;
    uint16_t startY;
    uint16_t endX;
    uint16_t endY;
    int8_t incX;
    int8_t incY;
    Operation operation;

    TranslationClip(
      char* Name,
      uint16_t startX,
      uint16_t startY,
      uint16_t endX,
      uint16_t endY,
      int8_t incX,
      int8_t incY,
      Operation operation
    ) : Clip(Name) {
      this->startX = startX;
      this->startY = startY;
      this->endX = endX;
      this->endY = endY;
      this->incX = incX;
      this->incY = incY;
      this->operation = operation;
      this->reset();
    }

    void reset() override {
      ESP_LOGI(I3_TRANSLATION_CLIP_TAG, "reset(%s)", this->name);
      this->x = this->startX;
      this->y = this->startY;
      this->isFinished = false;
    }

    void playFrame() override {
      //ESP_LOGI(I3_TRANSLATION_CLIP_TAG, "playFrame(%s)", this->name);

      this->operation(this->x, this->y);

      this->x += this->incX;
      this->y += this->incY;

      if ((this->x == this->endX) || (this->y == this->endY)){
        this->isFinished = true;
        //ESP_LOGI(I3_TRANSLATION_CLIP_TAG, "playFrame(%s) isFinished=true", this->name);        
      }
    }
};

#endif
