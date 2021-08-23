#include "Bullet.h"
#include "Body.h"
#include "Alien.h"
#include "Ship.h"
#include "Space.h"

#include <numbers>

tei::resources::Texture* Bullet::m_pTexture{};
tei::resources::Sound* Bullet::m_pSound{};

void Bullet::Hit(Body& body) const
{
	if (m_IsHostile)
	{
		if (auto ship{ body.GetParent<Ship>() })
		{
			ship->Death();
			m_pBody->Death();
		}
	}
	else
	{
		if (auto alien{ body.GetParent<Alien>() })
		{
			alien->Death();
			m_pBody->Death();
		}
	}
}

void Bullet::SetHostile(bool isHostile)
{
	m_IsHostile = isHostile;
}

void Bullet::OnEnable()
{
	if (auto& body{ Parent().GetComponent<Body>() })
	{
		m_pBody = &body;
		body.SetParent(*this);
	}
	else throw std::runtime_error{ "Bullet was not given a body" };

	if (!m_pTexture)
		m_pTexture = &tei::Resources->Load<tei::resources::Texture>("Resources/Bullet2.png");

	if (!m_pSound)
		m_pSound = &tei::Resources->Load<tei::resources::Sound>("Resources/count.wav");
	tei::Audio->Play(*m_pSound);

}

void Bullet::OnUpdate()
{
	if (tei::Time->frame.now - m_Birth > 5_s)
		m_pBody->Death();

	auto& vel = m_pBody->Velocity();
	m_pBody->TransformRef().rotation.x = atan2(vel.y, vel.x) + std::numbers::pi_v<float> / 2;
}

void Bullet::OnRender() const
{
	tei::Renderer->DrawTexture(*m_pTexture, m_pBody->Transform());
}

Bullet& SpawnBullet(Space& space, units::Position position, units::Vec2 velocity, bool hostile)
{
	auto& object = space.Parent().AddChild();

	auto& bullet = object.AddComponent<Bullet>();
	bullet.SetHostile(hostile);

	auto& body = object.AddComponent<Body>();
	body.SetSpace(space);
	
	body.VelocityRef() = velocity;
	auto& transform = body.TransformRef(); 
	transform.position = position;
	transform.rotation.x = atan2f(velocity.y, velocity.x);
	transform.scale = { .01f, .01f };

	body.SetParent(bullet);
	object.Enable();

	return bullet;
}
