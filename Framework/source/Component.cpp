#include "Component.h";

//Project Include
#include "Entity.h"

Component::Component(Entity* pOwner)
{
	m_ownerEntity = pOwner;
	m_componentType = NONE;
}

Component::~Component()
{
}
