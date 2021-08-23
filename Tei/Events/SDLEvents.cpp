#include "SDLEvents.h"
#include "../Application/Application.h"

namespace tei::internal::events
{

	void PollSDLEvents()
	{
		METRICS_TIMEBLOCK;

		SDL_Event event{};
		while (SDL_PollEvent(&event) != NULL)
		{
			if (event.type == SDL_QUIT)
				application::ApplicationService->Quit();

			Events->PushEvent(SDLEvent{ std::move(event) });
		}
	}

}