#pragma once

#include "Clock.h"

namespace tei::internal::time
{

	struct TimeOject
	{

		struct
		{
			Clock::time_point			now          {};
			Clock::time_point			startup      {};
		}
		global;

		struct
		{
			Clock::time_point			now          {};
			Clock::duration				delta        {};
			Clock::duration				lag          {};

			Clock::duration				step         { 1.f / 25.f };
			bool						fixed        { false };
		}
		frame;

	};

	extern utility::Static<TimeOject const> Time;

}

namespace tei::external
{

	using tei::internal::time::Clock;

	constexpr static auto const& Time{ tei::internal::time::Time };

}
