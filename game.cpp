#include "game.h"
#include <random>
#include <cmath>
#include <FriedMusicPlayer.h>
#include "ADKeyboardDectector.h"



void Game::Initialize()
{
	grid = Grid();
	gameOver = false;
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	// CheckFreezeBlock(&currentBlock);
	ApplyShadow();
	nextBlock = GetRandomBlock();
	start_audio("/Assets/Audio/tetris.raw");
	// InitAudioDevice();
	// music = LoadMusicStream("Assets/Audio/Tetris.mp3");
	// PlayMusicStream(music);
	gameSpeed = baseSpeed;

	rotateSound = load_sfx("/Assets/Audio/rotate.raw");
	clearSound = load_sfx("/Assets/Audio/clear.raw");
	loseSound = load_sfx("/Assets/Audio/lose.raw");
	winSound = load_sfx("/Assets/Audio/win.raw");
	dropSound = load_sfx("/Assets/Audio/drop.raw");
	lockSound = load_sfx("/Assets/Audio/lock.raw");
	cantSound = load_sfx("/Assets/Audio/cant.raw");
	clickSound = load_sfx("/Assets/Audio/click.raw");
}

void Game::PlayClick()
{
	play_sfx(clickSound);
}
void Game::PlayLock()
{
	play_sfx(lockSound);
}

void Game::ApplyShadow()
{
	//set the shadow to be the same type of block as currentBlock
	currentBlockShadow = Block(currentBlock);
	currentBlockShadow.id += 9; //add 9 to the id to give it a darker color
	DropShadow();
}

void Game::DropShadow()
{
	//set the shadow to the exact position as the block
	currentBlockShadow.rowOffset = currentBlock.rowOffset;
	currentBlockShadow.colummnOffset = currentBlock.colummnOffset;

	//keep moving the shadow down until it hit something
	while (!IsBlockOutside(&currentBlockShadow) && BlockFits(&currentBlockShadow))
	{
		currentBlockShadow.Move(1, 0);
	}
	//move it back up one to unclip it
	currentBlockShadow.Move(-1, 0);
}

void Game::UpdateGameSpeed(int linesCleared)
{
	gameSpeed -= 0.0012f * (linesCleared*2); // Increase speed every 10 lines
	if (gameSpeed < minSpeed)
	{
		gameSpeed = minSpeed;
	}
}

Block Game::GetRandomBlock()
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

std::vector<Block> Game::GetAllBlocks()
{
	return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}
void Game::DrawBlock(TFT_eSprite &spr, Block *pBlock)
{
	pBlock->Draw(spr);
}
void Game::DrawBlockUI(TFT_eSprite &spr, Block* pBlock, int offX, int offY)
{
	pBlock->DrawUI(spr, offX, offY);
}

void Game::DrawGrid(TFT_eSprite &spr)
{
	grid.Draw(spr);
}


void Game::Draw(TFT_eSprite &spr)
{
	DrawGrid(spr);

	DrawBlock(spr, &currentBlock);
	DrawBlock(spr, &currentBlockShadow);

	switch (nextBlock.id)
	{
	case 3: //i block
		DrawBlockUI(spr, &nextBlock, ((Columns - 2) * CellSize) - 5, (6 * CellSize) - 5);
		break;
	case 4: //o block
		DrawBlockUI(spr, &nextBlock, ((Columns - 2) * CellSize) - 5, (5 * CellSize) + 10);
		break;
	case 8: //d block
		DrawBlockUI(spr, &nextBlock, ((Columns - 3) * CellSize) + 5, (6 * CellSize) - 5);
		break;
	default:
		DrawBlockUI(spr, &nextBlock, ((Columns-2)*CellSize)+10, (5*CellSize)+10);
		break;
	}
}

void Game::HandleMovement()
{
	if (gameOver)
	{
		return;
	}
	if (ShouldMoveBlockDown() && IsKeyDown(KEY_DOWN))
	{
		UpdateScore(0, 1);
		MoveBlockDown();
		PlayClick();
	}
}

void Game::HandleInput()
{
	KEY keyPressed = GetKeyPressed();

	if (gameOver)
	{
		return;
	}

	switch (keyPressed)
	{
	case KEY_LEFT:
		MoveBlockLeft();
		break;
	case KEY_RIGHT:
		MoveBlockRight();
		break;
	case KEY_DOWN:
		if (!ShouldMoveBlockDown())
		{
			UpdateScore(0, 1);
			MoveBlockDown();
			PlayClick();
		}
		break;
	case KEY_SELECT:
		DropBlockDown();
		break;

	// case KEY_Q:
	// 	RotateLeft();
	// 	break;
	// case KEY_E:
	case KEY_UP:
		RotateRight();
		break;

	default:
		break;
	}
}

void Game::Move(int rows, int columns)
{
	currentBlock.Move(rows, columns);
	DropShadow();
}

void Game::MoveBlockLeft()
{
	if (gameOver)
	{
		return;
	}
	Move(0, -1);
	if (IsBlockOutside(&currentBlock) || !BlockFits(&currentBlock))
	{
		Move(0, 1);
		play_sfx(cantSound);
	}
}

void Game::MoveBlockRight()
{
	if (gameOver)
	{
		return;
	}
	Move(0, 1);
	if (IsBlockOutside(&currentBlock) || !BlockFits(&currentBlock))
	{
		Move(0, -1);
		play_sfx(cantSound);
	}
}

void Game::DropBlockDown()
{
	if (gameOver)
	{
		return;
	}
	DropShadow(); //drop the shadow again to make sure it is up to date

	//calculate the distance it will drop
	int dropDistance = currentBlockShadow.rowOffset - currentBlock.rowOffset;

	play_sfx(dropSound);
	//drop the block by overwriting its y value
	currentBlock.rowOffset = currentBlockShadow.rowOffset;
	
	//update the socre by twice the dropdistance because you did it instantly
	UpdateScore(0, dropDistance * 2);
}

bool Game::ShouldMoveBlockDown()
{
	return true;
	// return currentBlock.powerup != BlockFreeze;
}
void Game::MoveBlockDown()
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

void Game::RotateLeft()
{
	if (gameOver)
	{
		return;
	}
	bool success = SRSRotateLeft(&currentBlock);
	if (success)
	{
		play_sfx(rotateSound);
		currentBlockShadow.RotateLeft();
		DropShadow();
	}
	else
	{
		play_sfx(cantSound);
	}
}

void Game::RotateRight()
{
	if (gameOver)
	{
		return;
	}
	bool success = SRSRotateRight(&currentBlock);
	if (success)
	{
		play_sfx(rotateSound);
		currentBlockShadow.RotateRight();
		DropShadow();
	}
	else
	{
		play_sfx(cantSound);
	}
}
bool Game::SRSRotateRight(Block* pBlock)
{
	int start = pBlock->rotationState;
	int end = start;
	end++;
	if (end == (int)pBlock->cells.size())
	{
		end = 0;
	}

	auto kick = rot(start, end);
	auto kickdata = pBlock->wallkick[kick];

	int orgRow = pBlock->rowOffset;
	int orgColumn = pBlock->colummnOffset;

	pBlock->RotateRight();
	bool success = false;
	for (Position offset: kickdata)
	{
		//tries++;
		pBlock->Move(offset.row, offset.column);
		if (IsBlockOutside(pBlock) || !BlockFits(pBlock))
		{
			pBlock->rowOffset = orgRow;
			pBlock->colummnOffset = orgColumn;
			continue;
		}
		else
		{
			success = true;
			break;
		}
	}
	//std::cout << "it took " << tries << " tries" << std::endl;

	if (!success)
	{
		pBlock->RotateLeft();
	}


	return success;
}


bool Game::SRSRotateLeft(Block* pBlock)
{
	int start = pBlock->rotationState;
	int end = start;
	end--;
	if (end == -1)
	{
		end = (int)pBlock->cells.size() - 1;
	}

	auto kick = rot(start, end);
	auto kickdata = pBlock->wallkick[kick];

	int orgRow = pBlock->rowOffset;
	int orgColumn = pBlock->colummnOffset;

	pBlock->RotateLeft();
	bool success = false;
	//int tries = 0;
	for (Position offset : kickdata)
	{
		//tries++;
		pBlock->Move(offset.row, offset.column);
		if (IsBlockOutside(pBlock) || !BlockFits(pBlock))
		{
			pBlock->rowOffset = orgRow;
			pBlock->colummnOffset = orgColumn;
			continue;
		}
		else
		{
			success = true;
			break;
		}
	}
	//std::cout << "it took " << tries << " tries" << std::endl;

	if (!success)
	{
		pBlock->RotateRight();
	}

	return success;
}


void Game::LockBlock()
{
	std::vector<Position> tiles = currentBlock.GetCellPositions();

	for (Position item : tiles)
	{
		grid.grid[item.row][item.column] = currentBlock.id;
		// grid.powerups[item.row][item.column] = currentBlock.powerup;
	}

	//next block stuff
	currentBlock = nextBlock;
	// CheckFreezeBlock(&currentBlock);
	ApplyShadow();

	//game over
	if (!BlockFits(&currentBlock))
	{
		gameOver = true;
		stop_audio();
		// StopMusicStream(music);
		// play_sfx(loseSound);
	}

	//put the block and continue
	play_sfx(lockSound);
	nextBlock = GetRandomBlock();

	//clear any rows and reward points
	int rowsCleared = grid.ClearFullRows();
	ApplyClearPoints(rowsCleared);
}

void Game::ApplyClearPoints(int rowsCleared)
{
	UpdateGameSpeed(rowsCleared);
	UpdateScore(rowsCleared, 0);
	if (rowsCleared > 2)
	{
		play_sfx(winSound);
		play_sfx(clearSound);
	}
	else if (rowsCleared > 0)
	{
		play_sfx(clearSound);
	}
}

bool Game::IsBlockOutside(Block* pBlock)
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

bool Game::BlockFits(Block *pBlock)
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

void Game::Reset()
{
	grid.Initialize();
	gameSpeed = baseSpeed;
	gameOver = false;
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	ApplyShadow();
	score = 0;
	//PlayMusicStream(music);
	start_audio("/Assets/Audio/tetris.raw");
}


int GetRandomValue(int min, int max)
{
    static std::mt19937 gen(std::random_device{}()); // static so seeded once
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
	switch (linesCleared)
	{
	case 1:
		score += 100;
		break;
	case 2:
		score += 300;
		break;
	case 3:
		score += 500;
		break;
	case 4:
		score += 1000;
		break;
	default:
		break;
	}

	score += moveDownPoints;
}
