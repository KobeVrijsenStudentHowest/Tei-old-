#pragma once
#include <Tei.h>
#include <array>

class Game;
class Space;

class Level : public tei::Component
{
public:

	Level();
	~Level();

	void SetGame(Game&);

	void ShowText();

	void OnReturn();
	void OnWin();
	void OnDeath();
	void OnAdvance();

private:

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	Game* m_pGame{};
	tei::GameObject* m_pScene{};
	Space* m_pSpace{};
	int m_Level{};
	tei::Clock::time_point m_ReturnBy{};
	tei::prefab::TextComponent* m_pTimerText{};
	bool m_SecondUpdate{};
	bool m_HasWon{};
	int m_Score{};
	int m_Lives{ 3 };

};
