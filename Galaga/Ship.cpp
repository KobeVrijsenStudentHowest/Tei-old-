#include "Ship.h"
#include "Body.h"
#include "Space.h"
#include "Bullet.h"
#include "Explosion.h"

tei::resources::Texture* Ship::m_pTexture{};

void Ship::Death()
{
	SpawnExplosion(*m_pSpace, *m_pBody);
	if ((*m_pLives)-- > 0)
		return;
	tei::Events->PushEvent(PlayerDeathEvent{});
	m_pBody->Death();
}

void Ship::SetLives(int& lives)
{
	m_pLives = &lives;
}

void Ship::OnEnable()
{
	if (auto & body{ Parent().GetComponent<Body>() })
	{
		body.SetParent(*this);
		m_pBody = &body;
	}
	else throw std::runtime_error{ "Ship was not given a body" };
	if (auto& space{ Parent().Parent().GetComponent<Space>() })
	{
		m_pSpace = &space;
	}
	else throw std::runtime_error{ "Ship could not be coupled to space" };

	if (!m_pTexture)
		m_pTexture = &tei::Resources->Load<tei::resources::Texture>("Resources/ship.png");
}

void Ship::OnUpdate()
{
	if (abs(m_pBody->Transform().position.x) > .9f)
		m_pBody->SetGoal({ copysign(.9f, m_pBody->Transform().position.x), m_pBody->Transform().position.y });
}

void Ship::OnRender() const
{
	tei::Renderer->DrawTexture(*m_pTexture, m_pBody->Transform());

	if (auto transform = m_pBody->Transform(); *m_pLives > 0)
	{
		transform.position = { -1.f, 1.2f };
		transform.scale *= .8f;
		transform.rotation = {};
		for (size_t i{}; i < size_t(*m_pLives); ++i)
		{
			transform.position.x += .1f;
			tei::Renderer->DrawTexture(*m_pTexture, transform);
		}
	}
}

void Controller::SetControlledBody(Body& body)
{
	m_pBody = &body;
}

void Controller::OnEnable()
{
	constexpr float velocity = 1.f;

	auto& object = Parent();
	auto& observer = object.GetComponent<tei::Observer>();
	auto& space = object.Parent().GetComponent<Space>();
	m_pSpace = &space;

	auto& body = *m_pBody;
	body.VelocityRef() = {};

	observer.AddListener(tei::KeyboardInput{ tei::KEYCODE::LEFT, 
		[&body, velocity] (bool state)
		{
			if (state == false)
			{
				if (body.Velocity().x < 0)
					body.VelocityRef() = {};
			}
			else body.VelocityRef().x = -velocity;
		}
	});
	observer.AddListener(tei::KeyboardInput{ tei::KEYCODE::RIGHT,
		[&body, velocity] (bool state)
		{
			if (state == false)
			{
				if (body.Velocity().x > 0)
					body.VelocityRef() = {};
			}
			else body.VelocityRef().x = velocity;
		}
	});

	observer.AddListener(tei::KeyboardInput{ tei::KEYCODE::LCTRL,
		[this] (bool state)
		{ 
			m_Shooting = state;
		}
	});

	observer.AddListener(tei::Trigger<PlayerDeathEvent>{
		[this] 
		{
			Parent().Disable();
		}
	});

}

void Controller::OnUpdate()
{
	if (m_Shooting && m_ShotTimer < tei::Time->frame.now)
	{
		m_ShotTimer = tei::Time->frame.now + 150_ms;

		SpawnBullet(
			*m_pSpace,
			m_pBody->Transform().position,
			{ 0.f, -2.f },
			false
		);
	}
}

void Controller::OnDisable()
{
}

Controller& SpawnController(Space& space)
{
	auto& controllerObject = space.Parent().AddChild();
	auto& controller = controllerObject.AddComponent<Controller>();
	controllerObject.AddComponent<tei::Observer>();
	return controller;
}

Ship& SpawnShip(Space& space, Controller& controller, int& lives)
{
	auto& shipObject = space.Parent().AddChild();
	auto& body = shipObject.AddComponent<Body>();
	body.SetSpace(space);
	body.TransformRef().scale    = { .05f, .05f };
	body.TransformRef().position = { 0.f , 1.f  };
	auto& ship = shipObject.AddComponent<Ship>();
	controller.SetControlledBody(body);
	body.SetParent(ship);
	ship.SetLives(lives);
	return ship;
}
