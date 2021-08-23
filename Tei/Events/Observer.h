#pragma once
#include "../Components/Component.h"
#include "Trigger.h"

namespace tei::internal::events
{

	class Observer : public components::Component
	{
	public:

		template <typename CommandType>
		void AddListener(CommandType command)
		{
			m_Commands.push_back(std::unique_ptr<TriggerBase>(new CommandType{ std::move(command) }));
		}

	private:

		virtual void OnUpdate() override;

		std::vector<std::unique_ptr<TriggerBase>> m_Commands;

	};

}

