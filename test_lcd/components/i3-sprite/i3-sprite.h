#ifndef I3_SPRITE_H
#define I3_SPRITE_H

#define I3_SPRITE_TAG "I3-SPRITE"

#include <i3-lcd.h>

static uint8_t* spriteBuffer      = NULL;
static uint16_t spriteImageWidth  = 0;
static uint16_t spriteImageHeight = 0;
static uint8_t  (*spritePalette)[3];

void i3SpriteInit(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t palette[][3]);
uint8_t i3SpriteGetPixel(Pos pos);
void i3SpriteDraw(Pos src, Pos dst, Pos dstSize, bool xReverse=false, bool transparency=true, uint8_t size=1, uint16_t* buffer=NULL);

#endif
