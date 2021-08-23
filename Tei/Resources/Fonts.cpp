#include "Fonts.h"


#include "../Libs/Sdl.h"

using namespace tei::internal::resources;

prefab::FontsLoader::FontsLoader()
{
   if(TTF_Init() == -1)
       throw std::runtime_error{ "Failed to load SDL_ttf: "s + SDL_GetError() };
}

Font* prefab::FontsLoader::OnLoad(fs::path path, ResourceManager const& resources, std::optional<Font> init) const
{
    auto const file = resources.GetExisting(path).string();

    if (!init) init = Font{ .ptSize = 27 };

    if ((init->pData = TTF_OpenFont(file.c_str(), init->ptSize)) == nullptr)
        throw std::runtime_error{ "Failed to load font: " + file + ": " + SDL_GetError() };

    return new Font{ *move(init) };
}

void prefab::FontsLoader::OnFree(ResourceManager const&, Font* font) const
{
    TTF_CloseFont(static_cast<TTF_Font*>(font->pData));
    delete font;
}
