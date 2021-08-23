#pragma once

#include "../Components.h"
#include "../../Resources/Resources.h"
#include "../../Units/Units.h"

namespace tei::internal::components::prefab
{

	class TextComponent final : public Component
	{
	public:

		TextComponent();
		~TextComponent();

		inline std::string const& Text() const noexcept
		{ return m_Text; }

		inline std::string & TextRef() noexcept
		{ return m_TextUpdate = true, m_Text; }

		inline units::Colour const& Colour() const noexcept
		{ return m_Colour; }

		inline units::Colour & ColourRef() noexcept
		{ return m_TextUpdate = true, m_Colour; }

		inline units::Transform const& Transform() const noexcept
		{ return m_Transform; }

		inline units::Transform & TransformRef() noexcept
		{ return m_Transform; }

		inline std::string const& Font() const noexcept
		{ return m_FontName; }
		
		inline std::string & FontRef() noexcept
		{ return m_FontUpdate = true, m_FontName; }

	private:

		virtual void OnUpdate() override;
		virtual void OnRender() const override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		
		static constexpr std::string_view DEFAULT_FONT = "consola";

		resources::Font* m_pFont{};
		resources::Texture m_Texture{};
		std::string m_Text{ "No text" };
		std::string m_FontName{ DEFAULT_FONT };
		bool m_TextUpdate{};
		bool m_FontUpdate{};
		units::Transform m_Transform{};
		units::Colour m_Colour{ 1, 1, 1 };

	};

}