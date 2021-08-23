#pragma once

#include <Tei.h>

namespace units = tei::units;

class Body;
class Space;

class Alien final : public tei::Component
{
public:

	enum struct Type {
		FLY, BEE, MOTH, _COUNT
	};

	Alien();

	void SetType(Type type);
	void SetLefty(bool lefty);

	void Death();

private:

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnUpdate() override;
	virtual void OnRender() const override;

	void OccupyGrid();
	void ReleaseGrid();

	Body* m_pBody{};
	Space* m_pSpace{};
	uint8_t m_GridRow{}, m_GridColumn{};
	bool m_IsGridOccupied{};
	bool m_IsLefty{};

	Type m_Type{};

	tei::resources::Sprite m_Sprite;
	static tei::resources::Texture* m_pTextures[int(Type::_COUNT)];

	tei::Clock::time_point m_Birth{ tei::Time->frame.now };

};

Alien& SpawnAlien(Space& space, Alien::Type type, bool lefty);