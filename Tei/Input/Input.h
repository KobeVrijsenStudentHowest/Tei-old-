#pragma once
#include "../Events/Events.h"
#include "../Time/Time.h"
#include "../Libs/Sdl_Keys.h"

namespace tei::internal::input
{

	using InputCallback = std::function<void(bool state)>;

	class KeyboardInput final : public events::TriggerBase
	{
	public:

		KeyboardInput(Keycode key, InputCallback callback);

	private:

		virtual void Check(events::Event const& e) const override final;

		Keycode m_Key;
		InputCallback m_Call;

	};

}

namespace tei::external
{

	using tei::internal::input::KeyboardInput;
	using tei::internal::input::KEYCODE;

}