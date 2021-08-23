#pragma once

#include "Event.h"
#include "Trigger.h"
#include "Observer.h"

namespace tei::external
{

	static constexpr auto const& Events{ tei::internal::events::Events };

	using tei::internal::events::Event;
	using tei::internal::events::Observer;
	using tei::internal::events::Trigger;

}