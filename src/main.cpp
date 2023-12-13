#include <stdlib.h>
#include "GameCore.h"

using namespace TextGame;

int main()
{
	PlayerState playerState = {};
	WorldState worldState = {};

	InitializeGame(playerState, worldState);

	while (!playerState.WantsToExit)
	{
		RenderGame(playerState, worldState);
		GetInput(playerState, worldState);
		UpdateGame(playerState, worldState);
	}

	CleanupGame(playerState, worldState);

	system("pause");

	return 0;
}