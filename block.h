#pragma once
#include "settings.h"

#include <Map>
#include "position.h"
// #include "PowerupType.h"

class Block
{
public:
	Block();
	void Draw(TFT_eSprite &spr);
	void DrawUI(TFT_eSprite &spr, int offsetX, int offsetY);
	void Move(int rows, int columns);
	std::vector<Position> GetCellPositions();
	void RotateRight();
	void RotateLeft();
	int id;
	std::map<int, std::vector<Position>> cells;
	std::map<int, std::vector<Position>> wallkick;
	int rotationState;

	int rowOffset;
	int colummnOffset;
	// PowerupType powerup;
	// std::vector<Color> colors;
private:
};

