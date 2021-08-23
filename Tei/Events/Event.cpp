#include "Event.h"
#include <vector>
#include "SDLEvents.h"

namespace tei::internal::events
{

	EventManager::Service Events{};

	bool EventManager::m_Active{};
	std::array<std::vector<std::unique_ptr<Event>>, 2> EventManager::m_Events{};

	void EventManager::Update()
	{
		PollSDLEvents();
		m_Events[m_Active ^= true].clear();
	}

}