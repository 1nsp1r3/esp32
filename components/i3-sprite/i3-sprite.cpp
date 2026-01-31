#include "i3-sprite.h"

/**
 *
 */
void i3SpriteInit(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t (*palette)[3]){
  spriteBuffer      = buffer;
  spriteImageWidth  = width;
  spriteImageHeight = height;
  spritePalette     = palette;  
}

/**
 *
 */
uint8_t i3SpriteGetPixel(Pos pos){
  return spriteBuffer[pos.y*spriteImageWidth+pos.x];
}

/**
 * Display a sprite
 */
void i3SpriteDraw(Pos src, Pos dst, Pos dstSize, bool xReverse, bool transparency, uint8_t size, uint16_t* buffer){
  for (uint16_t col=0;col<dstSize.x;col++){
    for (uint16_t row=0;row<dstSize.y;row++){
      Pos pos(src.x+col, src.y+row);
      uint8_t colorIndex = i3SpriteGetPixel(pos);
      if (transparency && (colorIndex == 0)) continue;
      uint16_t color = i3LcdGetColorFromPalette(spritePalette, colorIndex);
      uint16_t x = xReverse ? (dst.x+dstSize.x-1)-col : dst.x+col;
      i3LcdSetPixel({x, (uint16_t)(dst.y+row)}, color, buffer);
      /*for (uint16_t sy = 0; sy < size; sy++) {
        for (uint16_t sx = 0; sx < size; sx++) {
          i3LcdSetPixel(xDst + col * size + sx, yDst + row * size + sy, color, buffer);
        }
      }*/
    }
  }
}
