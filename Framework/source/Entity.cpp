#include "Entity.h"

//typedefs
typedef std::pair<const unsigned int, Entity*> EntityPair;

//Statics
unsigned int Entity::s_uEntityCount = 0;
std::map<const unsigned int, Entity*> Entity::s_xEntityList;

Entity::Entity()
{
	//Increment entity ID
	m_entityID = s_uEntityCount++;

	//Add this entity to our List
	s_xEntityList.insert(EntityPair(m_entityID, this));
}

void Entity::Update(float a_deltaTime)
{
	std::vector<Component*>::iterator iter;
	for (iter = m_componentList.begin(); iter < m_componentList.end(); ++iter)
	{
		Component* pComponent = *iter;
		if (pComponent)
		{
			pComponent->Update(a_deltaTime);
		}
	}
}

void Entity::Draw(Shader* a_Shader)
{
	std::vector<Component*>::iterator iter;
	for (iter = m_componentList.begin(); iter < m_componentList.end(); ++iter)
	{
		Component* pComponent = *iter;
		if (pComponent)
		{
			pComponent->Draw(a_Shader);
		}
	}
}

Component* Entity::FindComponentOfType(COMPONENT_TYPE a_eComponentType) const
{
	std::vector<Component*>::const_iterator iter;
	for (iter = m_componentList.begin(); iter < m_componentList.end(); ++iter)
	{
		Component* pComponent = *iter;
		if (pComponent && pComponent->GetComponentType() == a_eComponentType)
		{
			return pComponent;
		}
	}
	return nullptr;
}