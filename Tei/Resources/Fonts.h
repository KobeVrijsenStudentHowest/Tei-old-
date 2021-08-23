#pragma once

#include "ResourceManager.h"

namespace tei::internal::resources
{

	struct Font
	{
		void* pData;
		int ptSize;

		constexpr operator bool() const noexcept
		{ return this != nullptr && pData != nullptr; }
	};

	namespace prefab
	{

		class FontsLoader final : public ResourceManager::Loader<Font>
		{
		public:

			FontsLoader();

		private:

			Font* OnLoad(fs::path path, ResourceManager const& resources, std::optional<Font> init) const override;

			void OnFree(ResourceManager const& resources, Font* font) const override;

		};

	}

}

namespace tei::external::resources
{
	using tei::internal::resources::Font;
}

