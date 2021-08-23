#pragma once

#include <memory>

class Menu;
class Level;

class Game
{

public:

	Game();
	~Game();

	void Quit();
	void GotoMain();
	void GotoLevel();

private:

	std::unique_ptr<Menu> m_pMenu;
	std::unique_ptr<Level> m_pLevel;

};

