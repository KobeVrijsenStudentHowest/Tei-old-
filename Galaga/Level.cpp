#include "Level.h"
#include "Ship.h"
#include "Alien.h"
#include "Space.h"
#include "Level.h"
#include "Game.h"
#include "Events.h"
#include "Spawner.h"

Level::Level()
{}

Level::~Level()
{}

void Level::SetGame(Game& game)
{
	m_pGame = &game;
}

void Level::ShowText()
{
	auto& textObject = m_pScene->AddChild();
	auto& observer = textObject.AddComponent<tei::Observer>();

	// Score
	{
		auto& scoreText = textObject.AddComponent<tei::prefab::TextComponent>();
		scoreText.TextRef() = std::format("{:0>6}", m_Score);
		scoreText.TransformRef().position = { 0.f, -1.06f };
		scoreText.TransformRef().scale = { .06f, .06f };
		scoreText.ColourRef() = { 1.f, 1.f, 1.f };

		observer.AddListener(tei::Trigger<ScoreEvent>{
			[&scoreText, this, level = m_Level + 1] (ScoreEvent const& event) mutable
			{
				m_Score += event.Score() * level;
				scoreText.TextRef() = std::format("{:0>6}", m_Score);
			}
		});
	}

	// Level
	{
		auto& levelText = textObject.AddComponent<tei::prefab::TextComponent>();
		levelText.TextRef() = std::format("Level {:0>2}", m_Level + 1);
		levelText.TransformRef().position = { 0.f, -1.2f };
		levelText.TransformRef().scale = { .1f, .1f };
		levelText.ColourRef() = { 1.f, .2f, .2f };
	}

	// Timer
	{
		auto& timerText = textObject.AddComponent<tei::prefab::TextComponent>();
		timerText.TextRef() = std::format("000", m_Level + 1);
		timerText.TransformRef().position = { .85f, 1.2f };
		timerText.TransformRef().scale = { .06f, .06f };
		timerText.ColourRef() = { 1.f, 1.f, 1.f };
		m_pTimerText = &timerText;
	}
}

void Level::OnReturn()
{
	if (m_HasWon)
	{
		OnAdvance();
	}
	else
	{
		m_pScene->Disable(true);
		m_pGame->GotoMain();
	}
}

void Level::OnWin()
{
	m_HasWon = true;
	m_ReturnBy = tei::Time->frame.now + 2_s;

	// Win message
	{
		auto& object = m_pScene->AddChild();
		auto& lifeText = object.AddComponent<tei::prefab::TextComponent>();
		lifeText.TextRef() = std::format("Level {} cleared!", m_Level + 1);
		lifeText.TransformRef().position = { 0.f, 0.f };
		lifeText.TransformRef().scale = { .1f, .1f };
		lifeText.ColourRef() = { 0.2f, 1.f, 0.2f };
		object.Enable();
	}
}

void Level::OnDeath()
{
	m_ReturnBy = tei::Time->frame.now + 5_s;

	// Death message
	{
		auto& object = m_pScene->AddChild();
		auto& lifeText = object.AddComponent<tei::prefab::TextComponent>();
		lifeText.TextRef() = "You have lost!";
		lifeText.TransformRef().position = { 0.f, 0.f };
		lifeText.TransformRef().scale = { .1f, .1f };
		lifeText.ColourRef() = { 1.f, 1.f, 0.2f };
		object.Enable();
	}
}

void Level::OnAdvance()
{
	++m_Level;
	m_HasWon = false;

	m_pScene->Disable(true);
	Parent().Disable();
}

void Level::OnEnable()
{
	auto& scene = Parent().AddChild();
	m_pScene = &scene;

	// Space
	auto& space = scene.AddComponent<Space>();
	space.GridRef().Resize(7 + (m_Level * 1) / 2, 4 + m_Level / 2);
	m_pSpace = &space;

	// Player
	auto& controller = SpawnController(space);
	space.SetShip(SpawnShip(space, controller, m_Lives));
	m_ReturnBy = tei::Time->frame.now + 160_s;

	// Aliens
	scene.AddComponent<Spawner>().SetDifficulty(m_Level);

	// Events
	auto& observer = scene.AddComponent<tei::Observer>();
	observer.AddListener(tei::KeyboardInput{ tei::KEYCODE::ESCAPE, [this] (auto) { OnReturn(); } });
	observer.AddListener(tei::Trigger<PlayerDeathEvent>{ [this] { OnDeath(); } });
	observer.AddListener(tei::Trigger<PlayerWonEvent>{ [this] { OnWin(); } });

	// Text
	ShowText();

	scene.Enable();
}

void Level::OnDisable()
{
	Parent().Enable();
}

void Level::OnUpdate()
{
	auto left{ m_ReturnBy - tei::Time->frame.now };

	if (left < 0_s)
		OnReturn();

	bool even = fmod(left.count(), 2.f) < 1.f;

	// Enters once every second.
	if (std::exchange(m_SecondUpdate, even) != m_SecondUpdate)
	{
		if (left >= 5_s)
			m_pTimerText->TextRef() = std::format("{:0>3}", unsigned(left.count()) - 5);
		else
			m_pTimerText->TextRef() = "---";
	}
}

void Level::OnRender() const
{
}
