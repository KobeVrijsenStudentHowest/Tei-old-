
#include "GameObject.h"

#include <ranges>
#include <algorithm>

using namespace tei::internal::components;

#define Call(fn)													\
{																	\
	constexpr auto l = [] (auto& e) -> void { e->fn(); };			\
	std::ranges::for_each(m_Components, l, utility::Getter<1>);		\
	std::ranges::for_each(m_Children, l);							\
}

GameObject::~GameObject()
{
	METRICS_TIMEBLOCK;

	if (m_Enabled) 
		Call(Disable);

	m_AddedChildren.clear();
	m_Children.clear();
	m_Components.clear();
}

void GameObject::Update()
{
	METRICS_TIMEBLOCK;

	if (!m_Enabled && m_Clear)
		this->~GameObject();

	if (!empty(m_AddedChildren))
	{
		std::ranges::move(m_AddedChildren, std::back_inserter(m_Children));
		m_AddedChildren.clear();
	}

	if (m_Enabled != m_SetEnabled)
	{
		m_Enabled = m_SetEnabled;
		if (m_Enabled)
			Call(Enable)
		else
			Call(Disable)
	}

	if (m_Enabled)
		Call(Update)
}

void GameObject::Render() const
{
	METRICS_TIMEBLOCK;

	if (m_Enabled)
		Call(Render)
}

Component* GameObject::AddComponent(std::type_info const* id, Component* component)
{
	METRICS_TIMEBLOCK;

	if (m_Enabled)
		throw utility::TeiRuntimeError{ "Cannot modify active GameObject component list" };
	
	m_Components.push_back({ id, std::unique_ptr<Component>{ component } });
	component->m_pParent = this;

	return component;
}

GameObject& GameObject::AddChild()
{
	METRICS_TIMEBLOCK;

	auto gameObject = std::make_unique<GameObject>();
	gameObject->m_pParent = this;
	
	if (m_Enabled)
	{
		return *m_AddedChildren.emplace_back(std::move(gameObject));
	}
	else
	{
		return *m_Children.emplace_back(std::move(gameObject));
	}
	//	throw utility::TeiRuntimeError{ "Cannot modify active GameObject children list" };
}