#include "Component.h"
#include <typeinfo>

#include <iostream>


Component::Component()
{
	gameObject = NULL;
	m_Manager = &GameStateManager::Instance();
}


Component::~Component()
{
}


Component* Component::GetComp(const type_info& type)
{
	if (type.name() == typeid(*this).name() || type.name() == typeid(this).name())
	{
		return this;
	}

	return NULL;
}
