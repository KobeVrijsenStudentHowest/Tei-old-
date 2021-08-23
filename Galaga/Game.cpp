#include "Game.h"

#include "Level.h"
#include "Menu.h"

Game::Game()
{
	GotoMain();
}

Game::~Game()
{}

void Game::Quit()
{
	tei::Application->Quit();
}

void Game::GotoMain()
{
	puts("Returned to main menu");

	if (!empty(tei::Scenes->Scenes()))
		tei::Scenes->CurrentScene()->Disable(true);

	auto& scene = tei::Scenes->AddScene();
	scene->AddComponent<Menu>().SetGame(*this);
	tei::Scenes->SetScene(scene);
}

void Game::GotoLevel()
{
	puts("Starting level");

	tei::Scenes->CurrentScene()->Disable(true);

	auto& scene = tei::Scenes->AddScene();
	scene->AddComponent<Level>().SetGame(*this);
	tei::Scenes->SetScene(scene);
}
