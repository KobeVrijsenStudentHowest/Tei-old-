#pragma once

#include "ResourceManager.h"

namespace tei::internal::resources
{

	struct Sound
	{
		void* pData;
		std::string name;
		float volume{ .1f };
		bool loop{};

		constexpr operator bool() const noexcept
		{
			return this != nullptr && pData != nullptr;
		}
	};

	namespace prefab
	{

		class AudioLoader final : public ResourceManager::Loader<Sound>
		{
		public:

			AudioLoader();
			~AudioLoader();

		private:

			Sound* OnLoad(fs::path path, ResourceManager const& resources, std::optional<Sound> init) const override;

			void OnFree(ResourceManager const& resources, Sound* font) const override;

		};

	}

}

namespace tei::external::resources
{
	using tei::internal::resources::Sound;
}

