#pragma once

#include <typeinfo>
#include <concepts>
#include <vector>
#include <functional>
#include <variant>

#include "../Components/Component.h"
#include "../Utility/Utility.h"

namespace tei::internal::events
{

	class Event;

	class EventManager
	{
	public:

		void Update();

		auto const& GetEvents()
		{
			return m_Events[!m_Active];
		}

		template <typename EventType>
		void PushEvent(EventType event) const
		{
			m_Events[m_Active].push_back(std::unique_ptr<Event>{new EventType{ std::move(event) }});
		}



		using Service = utility::Service<EventManager const>;

	private:

		static bool m_Active;
		static std::array<std::vector<std::unique_ptr<Event>>, 2> m_Events;

	};

	extern EventManager::Service Events;

	class Event
	{
	public:

		virtual ~Event() = default;

		template <std::derived_from<Event> EventType>
		Event(EventType*)
			: m_Type{ &typeid(EventType) }
		{}

		constexpr virtual operator bool() const final
		{ return this != nullptr; }

		constexpr virtual std::type_info const* Type() const final
		{ return m_Type; }

		template <std::derived_from<Event> EventType>
		constexpr EventType const& Cast() const
		{
			if (&typeid(EventType) == m_Type)
				return StaticCast<EventType>();
			else
				return nullptr;
		}

		template <std::derived_from<Event> EventType>
		constexpr EventType const& StaticCast() const
		{
			return *static_cast<EventType const*>(this);
		}

	private:

		std::type_info const* m_Type;

	};

}