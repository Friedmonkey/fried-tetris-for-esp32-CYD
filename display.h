#pragma once
#include <TFT_eSPI.h>
#include "game_objects.h"

inline void DrawBlock(Block *pBlock)
{
	pBlock->Draw();
	if (!powerupsEnabled) return;
	if (pBlock->powerup != BlockNormal)
	{
		std::vector<Position> tiles = pBlock->GetCellPositions();
		for (Position item : tiles)
		{
			if (item.row < BufferRows) continue;
			int x = item.column * CellSize + GapSize + OffSet;
			int y = (item.row - BufferRows) * CellSize + GapSize + OffSet;
			DrawPowerUp(pBlock->powerup, x, y, (pBlock->id > 8));
		}
	}
}

inline void loop_draw(TFT_eSprite &framebuffer)
{
  framebuffer.fillSprite(TFT_DARKCYAN); // clear previous frame
  grid.Draw(framebuffer);
  framebuffer.pushSprite(0, 0);      // now push entire buffer to screen
}
