#pragma once
#include "Events.h"

#include <vector>

#include "../Libs/Sdl.h"

namespace tei::internal::events
{

	class SDLEvent : public Event
	{
	public:

		SDLEvent(SDL_Event event)
			: Event{ this }
			, m_Event{ std::move(event) }
		{}

		SDL_Event const& GetEvent() const
		{
			return m_Event;
		}

	private:

		SDL_Event m_Event;

	};

	void PollSDLEvents();

}