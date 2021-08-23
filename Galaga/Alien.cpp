#include "Alien.h"
#include "Body.h"
#include "Space.h"
#include "Events.h"
#include "Bullet.h"
#include "Explosion.h"
#include <numbers>

tei::resources::Texture* Alien::m_pTextures[int(Type::_COUNT)]{};

Alien::Alien()
	: m_Sprite{ *m_pTextures[0], 1.f / 10.f, 2, 1, true }
{}

void Alien::SetType(Type type)
{
	m_Type = type;
}

void Alien::SetLefty(bool lefty)
{
	m_IsLefty = lefty;
}

void Alien::Death()
{
	ReleaseGrid();
	tei::Events->PushEvent(ScoreEvent{ 100 * int(m_Type) + rand() % 13 * 3 });
	SpawnExplosion(*m_pSpace, *m_pBody);
	m_pBody->Death();
}

void Alien::OnEnable()
{
	auto& object{ Parent() };

	if (auto& body{ object.GetComponent<Body>() })
	{
		body.SetParent(*this);
		m_pBody = &body;
	}
	else throw std::runtime_error{ "Alien was not given a body" };

	if (auto& space{ object.Parent().GetComponent<Space>() })
	{
		m_pSpace = &space;
	}
	else throw std::runtime_error{ "Alien could not be coupled to space" };

	if (!m_pTextures[int(m_Type)])
		m_pTextures[int(m_Type)] = &tei::Resources->Load<tei::resources::Texture>(
			std::data<std::string_view>({
				"Resources/Fly2.png",
				"Resources/Bee2.png",
				"Resources/Moth2.png"
			})[int(m_Type)]
		);
	m_Sprite.texture = *m_pTextures[int(m_Type)];

	m_pBody->TransformRef().scale = { .05f, .05f };
	m_pBody->TransformRef().position = { m_IsLefty ? -1.f : 1.f, .5f };
}

void Alien::OnDisable()
{
	ReleaseGrid();
}

void Alien::OnUpdate()
{
	if (auto alive{ tei::Time->frame.now - m_Birth }; alive < 2_s)
	{
		constexpr auto curve = [](float t) noexcept -> units::Vec2
		{
			// Curve approximation of a loop
			// X(t) = (t + .11) * (t - .27) * (t - .4) * (t - .87) * (t - 1.13) * -50 + .6   
			// Y(t) = X(1 - pow(t, .6))
			// Where: 0 <= t <= 1
			t = std::clamp(t, 0.f, 1.f);
			float x = ((t + .11f) * (t - .27f) * (t - .4f) * (t - .87f) * (t - 1.13f)) * -50.f + 0.6f;
			t = 1.f - pow(t, .6f);
			float y = ((t + .11f) * (t - .27f) * (t - .4f) * (t - .87f) * (t - 1.13f)) * -50.f + 0.6f;
			return { x, y };
		};
		auto p = curve(alive.count() / 2.f);

		m_pBody->VelocityRef() = { 100.f, 0.f };
		m_pBody->SetGoal({
			copysign(p.y + .1f, m_pBody->Transform().position.x),
			.5f - p.x
		});
	}
	else OccupyGrid();

	auto& vel = m_pBody->Velocity();
	if (abs(vel.x + vel.y) < std::numeric_limits<float>::epsilon())
		m_pBody->TransformRef().rotation.x = {};
	else
		m_pBody->TransformRef().rotation.x = atan2(vel.y, vel.x) + std::numbers::pi_v<float> / 2;

	if (rand() % 650 == 0 && m_pSpace->IsShipAlive())
	{
		auto direction = normalize(m_pSpace->ShipPosition() - m_pBody->Transform().position);
		SpawnBullet(*m_pSpace, m_pBody->Transform().position, direction, true);
	}
}

void Alien::OnRender() const
{
	tei::Renderer->DrawSprite(m_Sprite, m_pBody->Transform());
}

void Alien::ReleaseGrid()
{
	if (m_IsGridOccupied)
		m_pSpace->GridRef()[m_GridRow][m_GridColumn] = m_IsGridOccupied = false;
}

void Alien::OccupyGrid()
{
	if (m_IsGridOccupied) 
		return;

	auto& grid{ m_pSpace->Grid() };

	auto const& gridvec{ grid.AsVector() };

	auto range =
	[&]
	{
		auto first = begin(grid[1]);
		auto half = begin(grid[(grid.Height() - 1) / 2 + 1]);
		switch (m_Type)
		{
			case Type::BEE:
				return std::ranges::subrange(first, half);
			case Type::FLY:
				return std::ranges::subrange(half, end(gridvec));
			case Type::MOTH:
			default:
				return std::ranges::subrange(begin(gridvec), first);
		}
	}
	();

	uint8_t off{ uint8_t(distance(begin(gridvec), begin(range))) };
	std::vector<uint8_t> indeces{};
	for (uint8_t i{}; i < size(range); ++i)
		if (gridvec[i + off] == false)
			indeces.push_back(i);

	if (empty(indeces))
		throw std::runtime_error{ "Space grid is full, cannot place alien" };

	m_GridRow = indeces[rand() % size(indeces)] + off;
	m_GridColumn = m_GridRow % grid.Width();
	m_GridRow /= uint8_t(grid.Width());

	m_pSpace->GridRef()[m_GridRow][m_GridColumn] = m_IsGridOccupied = true;

	m_pBody->VelocityRef() = { 1.f, 0.f };
	m_pBody->SetGoal(
		m_pBody->Transform().scale *
		units::Scale{
			m_GridColumn - grid.Width() / 2.f,
			m_GridRow - 4.f
		} * 3.f
	);
}

Alien& SpawnAlien(Space& space, Alien::Type type, bool lefty)
{
	auto& object = space.Parent().AddChild();

	auto& alien = object.AddComponent<Alien>();
	alien.SetType(type);
	alien.SetLefty(lefty);
	
	auto& body = object.AddComponent<Body>();
	body.SetParent(alien);
	body.SetSpace(space);
	
	object.Enable();
	return alien;
}