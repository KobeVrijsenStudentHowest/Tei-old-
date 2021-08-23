#pragma once
#include "Event.h"

#include <optional>
#include <variant>

namespace tei::internal::events
{

	class TriggerBase
	{
	public:

		virtual ~TriggerBase() = default;

		TriggerBase(std::type_info const* type)
			: m_Type{ type }
		{}

		void DoCheck(Event const& event) const
		{
			if (event.Type() == m_Type)
				this->Check(event);
		}

	private:

		virtual void Check(Event const&) const = 0;

		std::type_info const* m_Type;

	};

	template <typename EventType>
	class Trigger final : public TriggerBase
	{

		using EventCB = std::function<void(EventType const&)>;
		using VoidCB = std::function<void()>;

	public:

		using Callback = std::variant<VoidCB, EventCB>;
		using Predicate = std::optional<std::function<bool(EventType const&)>>;

		virtual ~Trigger() = default;

		Trigger(Callback callback, Predicate predicate = std::nullopt)
			: TriggerBase{ &typeid(EventType) }
			, m_Pred{ std::move(predicate) }
			, m_Call{ std::move(callback) }
		{}

	private:

		virtual void Check(Event const& e) const override final
		{
			EventType const& event = e.StaticCast<EventType>();
			if (m_Pred && !m_Pred.value()(event))
				return;
			if (std::holds_alternative<EventCB>(m_Call))
				get<EventCB>(m_Call)(event);
			else
				get<VoidCB>(m_Call)();
		}

		Predicate m_Pred;
		Callback m_Call;

	};

}