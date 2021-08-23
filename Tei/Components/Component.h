#pragma once

#include <vector>
#include <typeinfo>
#include <optional>
#include <memory>
#include <ranges>
#include <algorithm>

namespace tei::internal::components
{

	class GameObject;

	class Component
	{

	protected:

		explicit Component() = default;

	public:

		virtual ~Component() = default;

		virtual GameObject& Parent() const final;

		virtual constexpr operator bool() const noexcept final;

	private:

		virtual void Update() final;
		virtual void Render() const final;
		virtual void Enable() final;
		virtual void Disable() final;

		virtual void OnUpdate() {};
		virtual void OnRender() const {};

		virtual void OnEnable() {};
		virtual void OnDisable() {};

		Component(const Component& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(Component&& other) = delete;

		friend GameObject;

		GameObject* m_pParent{};

	};

}

#pragma region _impl

namespace tei::internal::components
{

	inline void Component::Update()
	{
		this->OnUpdate();
	}

	inline void Component::Render() const
	{
		this->OnRender();
	}

	inline void Component::Enable()
	{
		this->OnEnable();
	}

	inline void Component::Disable()
	{
		this->OnDisable();
	}

	inline GameObject& Component::Parent() const
	{
		return *m_pParent;
	}

	inline constexpr Component::operator bool() const noexcept
	{
		return this != nullptr && m_pParent != nullptr;
	}

}

#pragma endregion