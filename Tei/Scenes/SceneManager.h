#pragma once

#include <vector>
#include <memory>

#include "../Components/GameObject.h"

namespace tei::internal::scenes
{

	class SceneManager;

	class Scene
	{

		Scene() = default;

	public:

		auto* operator -> () const noexcept
		{ return &m_Object; }

	private:

		void Update();
		void Render();

		mutable components::GameObject m_Object{};

		friend SceneManager;

	};

	class SceneManager
	{

	public:

		explicit SceneManager() = default;
		~SceneManager();

		inline Scene & CurrentScene() noexcept
		{ return *m_Scenes[m_CurrentScene]; }

		inline Scene const& CurrentScene() const noexcept
		{ return *m_Scenes[m_CurrentScene]; }

		inline auto const& Scenes() const noexcept
		{ return m_Scenes; }

		void SetScene(Scene const& scene) const;

		Scene const& AddScene() const noexcept;

		void Update();
		void Render();

	private:

		SceneManager(SceneManager const&) = delete;
		SceneManager(SceneManager &&) = delete;
		SceneManager& operator = (SceneManager const&) = delete;
		SceneManager& operator = (SceneManager &&) = delete;

		size_t m_CurrentScene{ ~size_t() };
		mutable size_t m_SetCurrentScene{};

		std::vector<std::unique_ptr<Scene>> m_Scenes{};
		mutable std::vector<std::unique_ptr<Scene>> m_AddedScenes{};

	public:

		using Service = utility::Service<SceneManager const>;

	};

	extern SceneManager::Service Scenes;

}

namespace tei::external
{
	constexpr static auto const& Scenes{ tei::internal::scenes::Scenes };

	using Scene = tei::internal::scenes::Scene;
}