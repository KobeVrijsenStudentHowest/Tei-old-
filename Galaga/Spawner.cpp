#include "Spawner.h"

#include "Space.h"

void Spawner::SetDifficulty(int level)
{
	m_Difficulty = level;
}

void Spawner::OnEnable()
{
	m_pSpace = &Parent().GetComponent<Space>();
	if (m_pSpace == nullptr)
		throw std::runtime_error{ "Spwaner could not be coupled to space" };

	int count = m_Difficulty * 5 + 5;
	for (auto v : std::ranges::views::iota(0, count))
		m_Queue.push({
			(rand() % 7 == 0) ^ (v >= 12) ? Alien::Type::BEE : Alien::Type::FLY,
			bool(v > count / 2)
		});
	for (auto v : std::ranges::views::iota(0, m_Difficulty))
		m_Queue.push({
			Alien::Type::MOTH,
			v % 2 == 0
		});
}

void Spawner::OnUpdate()
{

	if (m_Timer > 0)
	{
		m_Timer -= tei::Time->frame.delta.count();
	}
	else
	{
		m_Timer += m_Cooldown;

		if (!empty(m_Queue))
		{
			auto& toSpawn = m_Queue.front();
			SpawnAlien(*m_pSpace, toSpawn.type, toSpawn.lefty);
			m_Queue.pop();
		}
	}

}
