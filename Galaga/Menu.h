#pragma once

#include <Tei.h>

class Game;

class Menu : public tei::Component
{

public:

	void SetGame(Game&);

private:

	virtual void OnEnable() override;
	virtual void OnRender() const override;

	Game* m_pGame{};
	
	std::vector<tei::prefab::TextComponent*> m_Buttons{};
	size_t m_ButtonIndex{};
	
	static tei::resources::Texture* m_pSplash;
	static tei::resources::Texture* m_pTm;


};

