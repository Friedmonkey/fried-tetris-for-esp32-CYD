#pragma once
#include "grid.h"
#include "block.h"

#define BASE_SPEED 300
#define MIN_SPEED 80

extern bool gameOver;
extern int score;
extern float gameSpeed;

extern int jump_sound;
extern Grid grid;

extern Block currentBlock;
extern Block currentBlockShadow;
extern Block nextBlock;

extern std::vector<Block> blocks;

static void Move(int rows, int columns)
{
	currentBlock.Move(rows, columns);
	DropShadow();
}
static void ShouldMoveBlockDown()
{
  return true;
}
static void MoveBlockDown()
{
	if (gameOver)
	{
		return;
	}

  Move(1, 0);
  if (IsBlockOutside(&currentBlock) || !BlockFits(&currentBlock))
  {
    Move(-1, 0);
    LockBlock();
  }

}