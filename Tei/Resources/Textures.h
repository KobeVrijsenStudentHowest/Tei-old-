#pragma once

#include "ResourceManager.h"
#include "../Time/Time.h"

namespace tei::internal::resources
{

	struct Texture
	{
		void* pData;
		int w, h;

		constexpr operator bool() const noexcept
		{ return this != nullptr && pData != nullptr; }
	};

	struct Sprite
	{
		std::reference_wrapper<Texture> texture;
		float step;
		int cols, rows;
		bool loop;
		float origin{ time::Time->frame.now.time_since_epoch().count() };
	};

	namespace prefab
	{

		class TextureLoader final : public ResourceManager::Loader<Texture>
		{
		public:

			TextureLoader();

		private:

			Texture* OnLoad(fs::path path, ResourceManager const& resources, std::optional<Texture> init) const override;

			void OnFree(ResourceManager const& resources, Texture* init) const override;

		};

	}

}

namespace tei::external::resources
{
	using tei::internal::resources::Texture;
	using tei::internal::resources::Sprite;
}