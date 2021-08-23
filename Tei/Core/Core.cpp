#include "Core.h"
#include "../Time/Time.h"
#include "../Application/Application.h"
#include "../External/Extern.h"
#include "../Renderer/Renderer.h"
#include "../Scenes/SceneManager.h"
#include "../Events/Events.h"
#include "../Audio/AudioHandler.h"
#include "../Audio/DebugAudio.h"
#include "../Audio/SDLAudio.h"

#include <concepts>
#include <thread>

using namespace tei::internal::core;
using namespace tei::internal::time;
using namespace std::literals;

bool Core::m_IsRunning{ true };

struct StopSignal {};

void Core::GameLoop()
{
	auto& time = Time->frame;
	auto current = time.now = Clock::now();
	while (m_IsRunning)
	{
		METRICS_TIMEBLOCK;

		Time->global.now = current = Clock::now();
		if (time.fixed)
		{
			time.lag = current - time.now;
			time.now += time.delta = time.step;

			FrameUpdate();

			if (m_IsRunning && time.lag < time.step)
				std::this_thread::sleep_until(time.now);
		}
		else
		{
			time.delta = current - time.now;
			time.now = current;
			time.lag = {};

			FrameUpdate();

			//if (m_IsRunning && time.synced)
			/* sync */
		}
	}
}

void Core::FrameUpdate()
{
	METRICS_TIMEBLOCK;

	// Update
	{
		events::Events->Update();

		if (!m_IsRunning) return;

		scenes::Scenes->Update();

		if (!m_IsRunning) return;

		audio::Audio->Update();
		renderer::Renderer->Update();
	}

	// Render
	{
		renderer::Renderer->Clear();
		scenes::Scenes->Render();
		renderer::Renderer->Present();
	}

	// debug
#if defined(DEBUG) || defined(_DEBUG)
	if (Time->global.now.time_since_epoch() > 30s)
		Stop();
#endif // DEBUG

}

void Core::Run()
{
	METRICS_TIMEBLOCK;

	// Services
	audio::Audio.Register(new audio::SDLAudio{});
	events::Events.Register(new events::EventManager{});
	resources::Resources.Register(new resources::ResourceManager{});
	scenes::Scenes.Register(new scenes::SceneManager{});

	// Resource loaders
	resources::Resources->AddLoader(new resources::prefab::TextureLoader{});
	resources::Resources->AddLoader(new resources::prefab::FontsLoader{});
	resources::Resources->AddLoader(new resources::prefab::AudioLoader{});

	// Client init
	TeiClientInit();

	// Game
	try
	{
		GameLoop();
	}
	catch (StopSignal)
	{}

	// Clear scenes
	scenes::Scenes.Register(nullptr);

	// Client quit
	TeiClientQuit();

	// Clear services
	resources::Resources.Register(nullptr);
	events::Events.Register(nullptr);
	audio::Audio.Register(nullptr);

	// End
}

void Core::Stop()
{
	m_IsRunning = false;
	throw StopSignal{};
}