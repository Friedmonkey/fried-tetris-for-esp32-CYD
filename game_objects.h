#pragma once
#include "grid.h"
#include "block.h"
#include "position.h"
#include <FriedMusicPlayer.h>
#include <random>

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

static int GetRandomValue(int min, int max)
{
    static std::mt19937 gen(std::random_device{}()); // static so seeded once
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

static Block GetRandomBlock()
{
	if (blocks.empty())
	{
		blocks = GetAllBlocks();
	}
	int randomIndex = GetRandomValue(0, blocks.size()-1);
	Block block = blocks[randomIndex];
	blocks.erase(blocks.begin() + randomIndex);

	return block;
}

static std::vector<Block> GetAllBlocks()
{
	return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}
static bool IsBlockOutside(Block* pBlock)
{
	std::vector<Position> tiles = pBlock->GetCellPositions();
	for (Position item : tiles)
	{
		if (grid.IsCellOutside(item.row, item.column))
		{
			return true;
		}
	}
	return false;
}
static bool BlockFits(Block *pBlock)
{
	std::vector<Position> tiles = pBlock->GetCellPositions();
	for (Position item : tiles)
	{
		if (!grid.IsCellEmpty(item.row, item.column))
		{
			return false;
		}
	}
	return true;
}

static void LockBlock()
{
	std::vector<Position> tiles = currentBlock.GetCellPositions();

  for (Position item : tiles)
  {
    grid.grid[item.row][item.column] = currentBlock.id;
    grid.powerups[item.row][item.column] = currentBlock.powerup;
  }

	//next block stuff
	currentBlock = nextBlock;
	// CheckFreezeBlock(&currentBlock);
	// ApplyShadow();

	//game over
	if (!BlockFits(&currentBlock))
	{
		gameOver = true;
    stop_audio();
    //play_sfx();
		//StopMusicStream(music);
		//PlaySound(loseSound);
	}

	//put the block and continue
	//PlaySound(lockSound);
	nextBlock = GetRandomBlock();

	//clear any rows and reward points
	int rowsCleared = grid.ClearFullRows();
	ApplyClearPoints(rowsCleared);
}
static void Move(int rows, int columns)
{
	currentBlock.Move(rows, columns);
	// DropShadow();
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