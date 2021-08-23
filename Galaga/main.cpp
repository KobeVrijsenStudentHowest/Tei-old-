
#include <Tei.h>

#include "Game.h"

static std::unique_ptr<Game> s_pGame;

void TeiClientInit()
{

	puts("Starting Galaga");

	s_pGame.reset(new Game{});

}

void TeiClientQuit()
{

	puts("Closing Galaga");

	s_pGame.reset();

}