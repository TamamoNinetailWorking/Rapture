#pragma once

BIFROST_NAMESPACE_BEGIN

template<class Component>
inline Component* CActor::SearchComponent() const
{
	Component* component = nullptr;
	for (auto& elem : *m_ComponentList)
	{
		component = DCast<Component*>(elem);
		if (component == nullptr) { continue; }

		return component;
	}

	return nullptr;
}


BIFROST_NAMESPACE_END