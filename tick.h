#pragma once
#include "game_objects.h"


inline void loop_tick()
{
		if (pGame->ShouldMoveBlockDown())
		{
			pGame->MoveBlockDown();
		}
}
