#pragma once

#include <Tei.h>

class Body;
class Space;

class Bullet final : public tei::Component
{
public:

	Bullet() = default;

	void Hit(Body&) const;

	void SetHostile(bool isHostile);

private:

	virtual void OnEnable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	Body* m_pBody{};
	tei::Clock::time_point m_Birth{ tei::Time->frame.now };

	bool m_IsHostile{};

	static tei::resources::Texture* m_pTexture;
	static tei::resources::Sound* m_pSound;
};

Bullet& SpawnBullet(Space& space, tei::units::Position position, tei::units::Vec2 velocity, bool hostile);
