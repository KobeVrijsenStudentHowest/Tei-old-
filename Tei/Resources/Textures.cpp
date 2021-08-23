#include "Textures.h"
#include "../Renderer/Renderer.h"
#include "../Libs/Sdl.h"

using namespace tei::internal::resources;

prefab::TextureLoader::TextureLoader()
{
	METRICS_TIMEBLOCK;

	int init{
		bool(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) &
		bool(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) 
	};
	if (!init)
		throw utility::TeiRuntimeError{ "Could not properly initialize SDL_image" };
}

Texture* prefab::TextureLoader::OnLoad(fs::path path, ResourceManager const& resources, std::optional<Texture> init) const
{
	METRICS_TIMEBLOCK;

	auto const file = resources.GetExisting(path);

	auto const pathStr{ file.string() };

	if (!init) init = Texture{};

	SDL_Texture* pTexture = IMG_LoadTexture(
		static_cast<SDL_Renderer*>(renderer::Renderer->RenderTraget().pData),
		pathStr.c_str()
	);
	init->pData = pTexture;

	if (pTexture == nullptr ||
		SDL_QueryTexture(
			pTexture, 
			nullptr,
			nullptr, 
			&init->w, 
			&init->h
		) != 0
	)
		throw utility::TeiRuntimeError{ "Could not load texture: " + pathStr + ": " + SDL_GetError() };

	return new Texture{ *std::move(init) };
}

void prefab::TextureLoader::OnFree(ResourceManager const&, Texture* texture) const
{
	METRICS_TIMEBLOCK;

	SDL_DestroyTexture(static_cast<SDL_Texture*>(texture->pData));
	delete texture;
}

