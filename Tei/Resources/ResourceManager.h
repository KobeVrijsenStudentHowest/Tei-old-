#pragma once

#include "../Utility/Utility.h"
#include <memory>
#include <any>

namespace tei::internal::resources
{

	class ResourceManager final
	{
		struct LoaderBase;

	public:

		template <typename T>
		class Loader;

		ResourceManager(fs::path defaultPath = fs::current_path());
		~ResourceManager();

		fs::path const& DefaultPath() const
		{ return m_DefaultPath; }

		template <typename T>
		T& Load(fs::path path, std::optional<T> init = std::nullopt) const;

		template<typename T>
		Loader<T>& AddLoader(Loader<T>* init);

		fs::path GetExisting(fs::path const& path) const;

	private:

		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator = (ResourceManager const&) = delete;
		ResourceManager& operator = (ResourceManager&&) = delete;

		fs::path m_DefaultPath;

		struct LoaderBase
		{
			virtual ~LoaderBase() = default;
		};

		std::vector<std::pair<std::type_info const*, std::unique_ptr<LoaderBase>>> m_Loaders;

		mutable std::vector<std::any> m_Resources;

	public:

		template <typename T>
		class Loader : LoaderBase
		{
		public:

			~Loader() = default;
			explicit Loader() = default;

		protected:

			using ResourceManager = ResourceManager;

		private:

			virtual T* OnLoad(fs::path path, ResourceManager const& resources, std::optional<T> init) const = 0;
			virtual void OnFree(ResourceManager const& resources, T* object) const = 0;

			virtual inline std::type_info const& ID() const noexcept final
			{
				return typeid(T);
			}

			friend ResourceManager;

			Loader(Loader const&) = delete;
			Loader(Loader&&) = delete;
			Loader& operator = (Loader const&) = delete;
			Loader& operator = (Loader&&) = delete;

		};

		using Service = utility::Service<ResourceManager const>;

	};

	extern ResourceManager::Service Resources;

}

namespace tei::external
{

	static constexpr auto const& Resources{ tei::internal::resources::Resources };

}

#pragma region _impl

namespace tei::internal::resources
{

	template<typename T>
	inline T& ResourceManager::Load(fs::path path, std::optional<T> init) const
	{
		METRICS_TIMEBLOCK;

		if (auto const it = std::ranges::find(m_Loaders, &typeid(T), utility::Getter<0>); it != end(m_Loaders))
		{
			auto const loader = static_cast<Loader<T>*>(it->second.get());
			auto resource = std::shared_ptr<T>{
				loader->OnLoad(path, *this, init),
				[loader, this] (T* object) 
				{ 
					loader->OnFree(*this, object); 
				}
			};
			m_Resources.push_back(resource);
			return *resource;
		}
		else throw utility::TeiRuntimeError{ "No loader for type "s + typeid(T).name() };
	}

	template<typename T>
	inline ResourceManager::Loader<T>& ResourceManager::AddLoader(Loader<T>* pLoader)
	{
		METRICS_TIMEBLOCK;

		auto& pair = m_Loaders.emplace_back();
		pair.first = &pLoader->ID();
		pair.second.reset(pLoader);
		return *pLoader;
	}

}

#pragma endregion