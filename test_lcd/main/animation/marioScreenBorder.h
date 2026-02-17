#include <i3-sprite.h>
#include "asset/mario.h"

Pos marioScreenPos = {0, 0};
uint8_t marioScreenBorderFrame = 7;
bool marioScreenBorderInvert = false;
uint8_t marioScreenBorderPhase = 0;

/**
 *
 */
void marioScreenBorderShowFrame(Pos pos, bool invert){
  i3SpriteDraw({marioFrame[marioScreenBorderFrame][0], marioFrame[marioScreenBorderFrame][1]}, pos, {31, 30}, invert);
}

/**
 *
 */
void marioScreenBorderPlayFrame(){
  marioScreenBorderShowFrame(marioScreenPos, marioScreenBorderInvert);

  switch(marioScreenBorderPhase){
    case 0: //Top Left -> Top Right
      marioScreenBorderFrame = 7;
      marioScreenBorderInvert = false;
      marioScreenPos.x++;
      if (marioScreenPos.x >= 319-31) marioScreenBorderPhase++;
      break;
    case 1: //Rotation
      marioScreenBorderFrame++;
      if (marioScreenBorderFrame >= 11) marioScreenBorderPhase++;
     break;
    case 2: //Top Right -> Bottom Right
      marioScreenBorderFrame = 11;
      marioScreenBorderInvert = false;
      marioScreenPos.y++;
      if (marioScreenPos.y >= 239-30) marioScreenBorderPhase++;
      break;
    case 3: //Rotation
      marioScreenBorderInvert = true;
      marioScreenBorderFrame--;
      if (marioScreenBorderFrame <= 7) marioScreenBorderPhase++;
     break;
    case 4: //Bottom Right -> Bottom Left
      marioScreenBorderFrame = 7;
      marioScreenBorderInvert = true;
      marioScreenPos.x--;
      if (marioScreenPos.x < 1) marioScreenBorderPhase++;
      break;
    case 5: //Rotation
      marioScreenBorderFrame--;
      if (marioScreenBorderFrame <= 0) marioScreenBorderPhase++;
      break;
    case 6: //Bottom Left -> Top Left
      marioScreenBorderFrame = 0;
      marioScreenBorderInvert = false;
      marioScreenPos.y--;
      if (marioScreenPos.y < 1) marioScreenBorderPhase++;
      break;
    case 7: //Rotation
      marioScreenBorderFrame++;
      if (marioScreenBorderFrame >= 7) marioScreenBorderPhase = 0;
      break;
  }
}
