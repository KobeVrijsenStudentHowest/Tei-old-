#pragma once

#include <Tei.h>

class Body;
class Space;

class Explosion : public tei::Component
{
public:

	Explosion();

	void SetTransform(tei::units::Transform);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	tei::Clock::time_point m_Death{};
	tei::units::Transform m_Transform{};
	tei::resources::Sprite m_Sprite;

	static tei::resources::Texture* m_pTexture;
	static tei::resources::Sound* m_pSound;

};

Explosion& SpawnExplosion(Space& space, Body& body);

