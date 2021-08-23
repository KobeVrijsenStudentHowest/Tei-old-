#include "Input.h"
#include "../Libs/Sdl.h"
#include "../Utility/Utility.h"
#include "../Events/SDLEvents.h"

namespace tei::internal::input
{

	KeyboardInput::KeyboardInput(Keycode key, InputCallback call)
		: TriggerBase(&typeid(events::SDLEvent))
		, m_Key{ key }
		, m_Call{ std::move(call) }
	{}

	void KeyboardInput::Check(events::Event const& e) const
	{
		METRICS_TIMEBLOCK;
		auto const& event = e.StaticCast<events::SDLEvent>().GetEvent();
		if (bool down{ event.type == SDL_KEYDOWN }; down || event.type == SDL_KEYUP)
			if (!event.key.repeat && event.key.keysym.sym == static_cast<SDL_Keycode>(m_Key))
				m_Call(down);
	}

}
