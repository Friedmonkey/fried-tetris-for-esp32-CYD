#include "game_objects.h"

bool gameOver = false;
int score = 0;
float gameSpeed = BASE_SPEED;


int jump_sound = 0;
Grid grid = Grid();

Block currentBlock;
Block currentBlockShadow;
Block nextBlock;

std::vector<Block> blocks;