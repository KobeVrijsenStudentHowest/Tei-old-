
#include "SceneManager.h"

namespace tei::internal::scenes
{
	SceneManager::Service Scenes{};
}

using namespace tei::internal::scenes;

void Scene::Update()
{
	m_Object.Update();
}
void Scene::Render()
{
	m_Object.Render();
}

SceneManager::~SceneManager()
{
	CurrentScene()->Disable();
	CurrentScene().Update();
}

void SceneManager::SetScene(Scene const& scene) const
{
	auto const toAddress = [](auto& p) { return std::to_address(p); };

	if (auto it1 = std::ranges::find(m_Scenes, &scene, toAddress); it1 != end(m_Scenes))
	{
		m_SetCurrentScene = distance(begin(m_Scenes), it1);
	}
	else if(auto it2 = std::ranges::find(m_AddedScenes, &scene, toAddress); it2 != end(m_AddedScenes))
	{
		m_SetCurrentScene = distance(begin(m_AddedScenes), it2) + size(m_Scenes);
	}
	else throw utility::TeiRuntimeError{ "Invalid scene object used" };
}

Scene const& SceneManager::AddScene() const noexcept
{
	return *m_AddedScenes.emplace_back(std::unique_ptr<Scene>(new Scene{}));
}

void SceneManager::Update()
{
	if (!empty(m_AddedScenes))
	{
		std::ranges::move(m_AddedScenes, std::back_inserter(m_Scenes));
		m_AddedScenes.clear();
	}

	if (empty(m_Scenes))
		throw utility::TeiRuntimeError{ "No scene has been created" };

	if (m_CurrentScene != m_SetCurrentScene)
	{
		if (m_CurrentScene != ~size_t())
			CurrentScene()->Disable();
		m_CurrentScene = m_SetCurrentScene;
		CurrentScene()->Enable();
	}

	CurrentScene().Update();
}

void SceneManager::Render()
{
	CurrentScene().Render();
}