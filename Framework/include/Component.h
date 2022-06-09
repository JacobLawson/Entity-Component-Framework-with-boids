#pragma once

//Forward Declarations
class Entity;
class Shader;

enum COMPONENT_TYPE
{
	NONE,
	TRANSFORM,
	MODEL,
	CAMERA,
	FREE_CAMERA,
	BRAIN,
};

class Component
{
public:
	Component(Entity* pOwner);
	~Component();

	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(Shader* pShader) = 0;

	inline Entity* GetOwnerEntity() { return m_ownerEntity; }
	inline COMPONENT_TYPE GetComponentType() { return m_componentType; }


protected:
	Entity* m_ownerEntity;
	COMPONENT_TYPE m_componentType;
};