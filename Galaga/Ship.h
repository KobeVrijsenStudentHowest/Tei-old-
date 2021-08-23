#pragma once

#include <Tei.h>

namespace units = tei::units;

class Body;
class Space;
class Texture;

class Ship final : public tei::Component
{
public:

	Ship() = default;

	void Death();
	void SetLives(int&);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	Body* m_pBody{};
	Space* m_pSpace{};
	int* m_pLives{};

	static tei::resources::Texture* m_pTexture;

};

class Controller final : public tei::Component
{
public:

	Controller() = default;

	void SetControlledBody(Body&);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;
	virtual void OnDisable() override;

	Body* m_pBody{};
	Space* m_pSpace{};

	tei::Clock::time_point m_ShotTimer{ tei::Time->frame.now };
	bool m_Shooting{};

};

class PlayerDeathEvent final : public tei::Event
{
public:
	PlayerDeathEvent()
		: Event{ this }
	{}
};

class PlayerWonEvent final : public tei::Event
{
public:
	PlayerWonEvent()
		: Event{ this }
	{}
};

Controller& SpawnController(Space& space);

Ship& SpawnShip(Space& space, Controller& controller, int& lives);