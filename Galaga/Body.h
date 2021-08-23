#pragma once

#include <Tei.h>
namespace units = tei::units;

class Space;
class Ship;
class Alien;
class Bullet;

class Body final : public tei::Component
{
public:

	Body() = default;

	auto const& Transform() const noexcept
	{ return m_Transform; }
	auto & TransformRef() noexcept
	{ return m_Transform; }

	auto const& Velocity() const noexcept
	{ return m_Velocity; }
	auto & VelocityRef() noexcept
	{ return m_Goal.reset(), m_Velocity; }

	void SetGoal(units::Position goal);

	template <typename Parent>
	void SetParent(Parent& parent)
	{ m_pParent = &parent; }
	
	template <typename Parent>
	Parent* GetParent() const
	{ 
		if (std::holds_alternative<Parent*>(m_pParent))
			return std::get<Parent*>(m_pParent);
		else 
			return nullptr;
	}

	void SetSpace(Space& space) noexcept
	{ m_pSpace = &space; }

	void Death() const;

private:

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate() override;

	units::Vec2 m_Velocity{};
	units::Transform m_Transform{};
	std::optional<units::Position> m_Goal{};

	std::variant<Ship*, Alien*, Bullet*> m_pParent{};

	Space* m_pSpace{};

};