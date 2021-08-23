#include "TextComponent.h"


#include <filesystem>

#include "../../Renderer/Renderer.h"
#include "../../Libs/Sdl.h"

namespace tei::internal
{

	namespace fs = std::filesystem;
	using namespace components::prefab;
	using namespace resources;

	SDL_Renderer* SDLRenderer()
	{
		return static_cast<SDL_Renderer*>(renderer::Renderer->RenderTraget().pData);
	}

	TextComponent::TextComponent()
	{}

	TextComponent::~TextComponent()
	{
		if (m_Texture)
			SDL_DestroyTexture(static_cast<SDL_Texture*>(m_Texture.pData));
	}

	void TextComponent::OnUpdate()
	{
		METRICS_TIMEBLOCK;

		if (m_FontUpdate || !m_pFont)
		{

#ifdef _WIN32

			// Default font windows
			fs::directory_iterator const dirIt{ "/Windows/Fonts/" };
			fs::path const fontFile = *std::ranges::find_if(
				dirIt,
				[](fs::path const& file) { return file == DEFAULT_FONT; },
				[](fs::path const& file) { return file.filename().replace_extension(); }
			);

#elif

			throw utility::TeiRuntimeError{ "No font could be found" };

#endif // _WIN32

			m_pFont = &Resources->Load<resources::Font>(fontFile);

			m_FontUpdate = false;
		}

		if (m_TextUpdate || !m_Texture)
		{
			SDL_Colour colour{ uint8_t(m_Colour.r * 255), uint8_t(m_Colour.g * 255), uint8_t(m_Colour.b * 255) };
			auto const surface = TTF_RenderText_Blended(static_cast<TTF_Font*>(m_pFont->pData), m_Text.c_str(), colour);
			if (surface == nullptr)
				throw utility::TeiRuntimeError{ "Surface could not be created from font " + m_FontName };

			auto const texture = SDL_CreateTextureFromSurface(SDLRenderer(), surface);
			SDL_FreeSurface(surface);
			if (texture == nullptr)
				throw utility::TeiRuntimeError{ "Texture could not be created from font " + m_FontName };

			SDL_QueryTexture(texture, nullptr, nullptr, &m_Texture.w, &m_Texture.h);
			m_Texture.pData = texture;

			m_TextUpdate = false;
		}
	}

	void TextComponent::OnRender() const
	{
		METRICS_TIMEBLOCK;

		renderer::Renderer->DrawTexture(m_Texture, m_Transform);
	}

	void TextComponent::OnEnable()
	{
	}

	void TextComponent::OnDisable()
	{
	}

}