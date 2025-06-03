#include <SDL.h>
#include <cstdlib>

#include "Game.h"

int main(int argumentCount, char * arguments[])
{
	Game game;
	game.Run();

	return EXIT_SUCCESS;
}