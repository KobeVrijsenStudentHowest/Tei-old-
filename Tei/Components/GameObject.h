#pragma once

#include "Component.h"
#include "../Utility/Utility.h"

namespace tei::internal::scenes
{
	class Scene;
}

namespace tei::internal::components
{

	class GameObject final
	{
	public:

		explicit GameObject() = default;
		~GameObject();

		explicit GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		GameObject& Parent() const noexcept;
		auto const& Children() const noexcept;
		auto const& Components() const noexcept;
		//TransformComponent& Transform() const noexcept;

		void Enable();
		void Disable(bool clear = false);
		void Clear();

		bool IsEnabled() const noexcept;
		operator bool() const noexcept;

		GameObject& AddChild();
		template <std::derived_from<Component> Comp> Comp& AddComponent();
		template <std::derived_from<Component> Comp> Comp& GetComponent() const;

	private:

		void Update();
		void Render() const;
		friend scenes::Scene;

		Component* AddComponent(std::type_info const*, Component*);

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		GameObject* m_pParent{};

		mutable bool m_SetEnabled{ false };
		mutable bool m_Clear{ false };
		bool m_Enabled{ false };

		std::vector<std::unique_ptr<GameObject>> m_Children{};
		mutable std::vector<std::unique_ptr<GameObject>> m_AddedChildren{};
		std::vector<std::pair<std::type_info const*, std::unique_ptr<Component>>> m_Components{};
	};

}

#pragma region _impl

namespace tei::internal::components
{

	inline GameObject& GameObject::Parent() const noexcept
	{
		return *m_pParent;
	}

	inline auto const& GameObject::Children() const noexcept
	{
		return m_Children;
	}

	inline auto const& GameObject::Components() const noexcept
	{
		return m_Components;
	}

	inline bool GameObject::IsEnabled() const noexcept
	{
		return m_Enabled;
	}

	inline void GameObject::Enable()
	{
		m_SetEnabled = true;
	}

	inline void GameObject::Clear()
	{
		if (m_Enabled)
			throw utility::TeiRuntimeError{ "Cannot clear enabled game object" };
		m_Clear = true;
	}

	inline void GameObject::Disable(bool clear)
	{
		m_SetEnabled = false;
		m_Clear |= clear;
	}

	inline GameObject::operator bool() const noexcept
	{
		return this != nullptr;
	}

	template <std::derived_from<Component> Comp>
	inline Comp& GameObject::GetComponent() const
	{
		return *static_cast<Comp*>(
			std::ranges::find(
				m_Components,
				&typeid(Comp),
				utility::Getter<0>
			)->second.get()
		);
	}

	template <std::derived_from<Component> Comp>
	inline Comp& GameObject::AddComponent()
	{
		static_assert(std::is_default_constructible_v<Comp>, "Tei: Components must be default constructable. Please provide an empty constructor.");
		return *static_cast<Comp*>(AddComponent(&typeid(Comp), m_Enabled ? nullptr : new Comp{}));
	}

}

#pragma endregion