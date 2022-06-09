#pragma once
#include "Component.h"

#include <vector>
#include <map>

//Forward Declarations
class Shader;

class Entity
{
public:
	Entity();
	~Entity();

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* a_shader);

	void AddComponent(Component* a_component) { m_componentList.push_back(a_component); }
	Component* FindComponentOfType(COMPONENT_TYPE a_eComponentType) const;

	unsigned int GetEntityID() { return m_entityID; }
	static const std::map<const unsigned int, Entity*>& GetEntityList() { return s_xEntityList; }

private:
	unsigned int m_entityID;
	std::vector<Component*> m_componentList;

	static unsigned int s_uEntityCount;
	static std::map<const unsigned int, Entity*> s_xEntityList;
};