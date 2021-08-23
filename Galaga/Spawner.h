#pragma once

#include <Tei.h>
#include <queue>

#include "Alien.h"

class Spawner : public tei::Component
{

public:

	void SetDifficulty(int level);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;

	float m_Cooldown = 0.1f;
	float m_Timer{};
	class Space* m_pSpace{};
	int m_Difficulty{};

	struct ToSpawn
	{
		Alien::Type type{};
		bool lefty{};
	};

	std::queue<ToSpawn> m_Queue{};

};

