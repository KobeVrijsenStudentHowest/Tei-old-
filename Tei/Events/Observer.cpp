#include "Observer.h"

namespace tei::internal::events
{

	void Observer::OnUpdate()
	{
		METRICS_TIMEBLOCK;
		for (auto& pEvent : Events->GetEvents())
			for (auto& pCommand : m_Commands)
				pCommand->DoCheck(*pEvent);
	}

}