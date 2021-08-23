#include "Menu.h"
#include "Game.h"

using namespace tei::units;

tei::resources::Texture* Menu::m_pSplash{};
tei::resources::Texture* Menu::m_pTm{};

void Menu::SetGame(Game& game)
{
	m_pGame = &game;
}

void Menu::OnEnable()
{
	auto& scene = Parent().AddChild();
	scene.Enable();

	auto& observer = scene.AddComponent<tei::Observer>();

	Scale scale{ .09f, .09f };
	Scale scaleSelected{ .11f, .11f };

	Transform transform{
		{ 0.f, .1f },
		scale
	};
	Vec2 transformShift{ 0.f, .2f };

	Colour colour{ .7f, .7f, .7f };
	Colour colourSelected{ 1.f, 1.f, .7f };

	{
		auto& text = scene.AddComponent<tei::prefab::TextComponent>();
		text.TextRef() = "PLAY";
		text.ColourRef() = { .7f, .7f, .7f };
		text.TransformRef() = transform;
		transform.position += transformShift;
		m_Buttons.push_back(&text);
	}

	{
		auto& text = scene.AddComponent<tei::prefab::TextComponent>();
		text.TextRef() = "QUIT";
		text.ColourRef() = { .7f, .7f, .7f };
		text.TransformRef() = transform;
		transform.position += transformShift;
		m_Buttons.push_back(&text);
	}

	m_Buttons[m_ButtonIndex]->ColourRef() = colourSelected;
	m_Buttons[m_ButtonIndex]->TransformRef().scale = scaleSelected;

	auto setButtonIndex =
	[=, this] (ptrdiff_t d)
	{
		m_Buttons[m_ButtonIndex]->ColourRef() = colour;
		m_Buttons[m_ButtonIndex]->TransformRef().scale = scale;
		m_ButtonIndex = std::clamp(m_ButtonIndex + d + 1, size_t(1), size(m_Buttons)) - 1;
		m_Buttons[m_ButtonIndex]->ColourRef() = colourSelected;
		m_Buttons[m_ButtonIndex]->TransformRef().scale = scaleSelected;
	};

	observer.AddListener(tei::KeyboardInput{
		tei::KEYCODE::DOWN,
		[=] (bool state) 
		{
			if (state)
				setButtonIndex(1);
		}
	});
	observer.AddListener(tei::KeyboardInput{
		tei::KEYCODE::UP,
		[=] (bool state) 
		{
			if (state)
				setButtonIndex(-1);
		}
	});
	observer.AddListener(tei::KeyboardInput{
		tei::KEYCODE::RETURN,
		[this] (bool state)
		{
			if (state)
			switch (m_ButtonIndex)
			{
			case 0:
				m_pGame->GotoLevel();
				break;
			case 1:
				m_pGame->Quit();
				break;
			}
			return;
		}
	});
	observer.AddListener(tei::KeyboardInput{
		tei::KEYCODE::ESCAPE,
		[this] (bool state) 
		{
			if (state)
				m_pGame->Quit();
		}
	});

	m_pSplash = &tei::Resources->Load<tei::resources::Texture>("Resources/galaga.png");
	m_pTm = &tei::Resources->Load<tei::resources::Texture>("Resources/tm.png");

	{
		auto& text = scene.AddComponent<tei::prefab::TextComponent>();
		text.TextRef() = "This is a replica created for educational use.";
		text.ColourRef() = { .7f, .7f, .7f };
		text.TransformRef() = { { -.237f, 1.15f }, { .028f, .028f } };
	}
}

void Menu::OnRender() const
{
	tei::Renderer->DrawTexture(*m_pSplash, { { 0.f, -.75f }, { .5f, .5f } });
	tei::Renderer->DrawTexture(*m_pTm, { { -.23f, 1.25f }, { .028f, .028f } });
}
