#include <i3-sprite.h>
#include "asset/mario.h"

uint8_t marioRotationFrame=0;

/**
 *
 */
void marioRotationShowFrame(uint16_t x, uint16_t y){
  i3SpriteDraw({marioFrame[marioRotationFrame][0], marioFrame[marioRotationFrame][1]}, {x, y}, {31, 30}, marioFrame[marioRotationFrame][2]);
}

/**
 *
 */
void marioRotationPlayFrame(){
  marioRotationShowFrame(160, 120);

  marioRotationFrame++;
  if (marioRotationFrame >= MARIO_MAX_FRAMES){
    marioRotationFrame=0;
  }
}
