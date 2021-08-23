#include "Explosion.h"
#include "Body.h"
#include "Space.h"

tei::resources::Texture* Explosion::m_pTexture{};
tei::resources::Sound* Explosion::m_pSound{};

Explosion::Explosion()
	: m_Sprite{ *m_pTexture, 1.f / 12.f, 6, 1, false }
{}

void Explosion::SetTransform(tei::units::Transform transform)
{
	m_Transform = std::move(transform);
}

void Explosion::OnEnable()
{
	if (!m_pTexture)
		m_pTexture = &tei::Resources->Load<tei::resources::Texture>(
			"Resources/Explosion.png"
		);
	m_Sprite.texture = *m_pTexture;

	if (!m_pSound)
		m_pSound = &tei::Resources->Load<tei::resources::Sound>(
			"Resources/explosion.wav"
		);

	tei::Audio->Play(*m_pSound);

	m_Death = tei::Time->frame.now + 1_s;
}

void Explosion::OnUpdate()
{
	if (m_Death < tei::Time->frame.now)
		Parent().Disable(true);
}

void Explosion::OnRender() const
{
	tei::Renderer->DrawSprite(m_Sprite, m_Transform);
}

Explosion& SpawnExplosion(Space& space, Body& body)
{
	auto& object = space.Parent().AddChild();
	auto& explosion = object.AddComponent<Explosion>();
	auto transform = body.Transform();
	transform.scale *= 2;
	explosion.SetTransform(std::move(transform));
	object.Enable();
	return explosion;
}
