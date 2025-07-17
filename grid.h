#pragma once

#include "settings.h"
// #include "PowerupType.h"
#include "Position.h"

class Grid
{
public:
	Grid();
	void Initialize();
	void Draw(TFT_eSprite &framebuffer);
	bool IsCellOutside(int row, int column);
	bool IsCellEmpty(int row, int column);
	void MoveColumnDown(int column, int startIndex, int amount);
	int ClearFullRows();
	int grid[Rows][Columns];
private:
	bool IsRowFull(int row);
	void ClearRow(int row);
	void MoveRowDown(int row, int amount);
};

