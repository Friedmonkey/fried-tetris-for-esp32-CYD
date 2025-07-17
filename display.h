#pragma once
#include <TFT_eSPI.h>
#include "game_objects.h"


inline void loop_draw(TFT_eSprite &framebuffer)
{
  framebuffer.fillSprite(TFT_DARKCYAN); // clear previous frame
  game.Draw(framebuffer);
  framebuffer.pushSprite(0, 0);      // now push entire buffer to screen
}
