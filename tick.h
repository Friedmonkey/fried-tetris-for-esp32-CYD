#pragma once
#include "game_objects.h"


inline void loop_tick()
{
		if (ShouldMoveBlockDown())
		{
			MoveBlockDown();
		}
}
