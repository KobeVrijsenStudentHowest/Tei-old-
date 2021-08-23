#include "Space.h"
#include "Body.h"
#include "Bullet.h"
#include "Level.h"
#include "Ship.h"

void Space::SetShip(Ship& ship) noexcept
{
	//m_pShip = &ship;
	m_pShipBody = &ship.Parent().GetComponent<Body>();
}

units::Position const& Space::ShipPosition() const noexcept
{
	return m_pShipBody->Transform().position;
}

bool Space::IsShipAlive() const noexcept
{
	return m_IsAlive;
}

void Space::AddBody(Body& body)
{
	m_Bodies.push_back(&body);
	if (body.GetParent<Alien>())
		++m_AlienCount;
}

void Space::RemoveBody(Body& body)
{
	if (auto it{ std::ranges::find(m_Bodies, &body) }; it != end(m_Bodies))
		m_Bodies.erase(it);
	if (body.GetParent<Alien>())
		--m_AlienCount;
}

bool IsOverlapping(units::Transform const& a, units::Transform const& b)
{
	auto min = (a.scale.x + a.scale.y + b.scale.x + b.scale.y) / 2;
	auto diff = b.position - a.position;
	return min * min > glm::dot(diff, diff);
}

void Space::OnEnable()
{
	auto& object = Parent().AddChild();
	object.AddComponent<tei::Observer>().AddListener(tei::Trigger<PlayerDeathEvent>{
		[this] { m_IsAlive = false; }
	});
	object.Enable();
}

void Space::OnUpdate()
{
	if (size(m_Bodies) > 1)
	{
		auto pos = begin(m_Bodies), last = end(m_Bodies);
		for (auto current : std::ranges::subrange(pos, prev(last)))
		for (auto other   : std::ranges::subrange(++pos, last))
		if (IsOverlapping(current->Transform(), other->Transform()))
		{
			if (auto bullet1{ current->GetParent<Bullet>() })
				bullet1->Hit(*other);
			else if (auto bullet2{ other->GetParent<Bullet>() })
				bullet2->Hit(*current);
		}
	}
	if (m_AlienCount <= 0 && (m_EmptyTime -= tei::Time->frame.delta) <= 0_s)
	{
		tei::Events->PushEvent(PlayerWonEvent{});
		m_EmptyTime = m_EmptyTime.max();
	}
}

void Space::OnRender() const
{}
