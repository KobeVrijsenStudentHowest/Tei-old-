#include "Body.h"
#include "Space.h"

void Body::SetGoal(units::Position goal)
{
	m_Goal = goal;
	m_Velocity = glm::normalize(goal - m_Transform.position) * glm::length(m_Velocity);
}

void Body::Death() const
{
	Parent().Disable(true);
}

void Body::OnEnable()
{
	m_pSpace->AddBody(*this);
}

void Body::OnDisable()
{
	m_pSpace->RemoveBody(*this);
}

void Body::OnUpdate()
{
	auto const delta{ m_Velocity * tei::Time->frame.delta.count() };
	if (m_Goal && distance(m_Goal.value(), +m_Transform.position) < length(delta))
	{
		m_Transform.position = m_Goal.value();
		m_Goal.reset();
		m_Velocity = {};
	}
	else m_Transform.position += delta;
}
