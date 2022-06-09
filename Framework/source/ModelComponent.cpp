#include "ModelComponent.h"

#include <learnopengl/include/shader.h>
#include <learnopengl/include/model.h>

#include <glm/gtc/matrix_transform.hpp>

#include <Entity.h>
#include <TransformComponent.h>

//typeDefs
typedef Component PARENT;

ModelComponent::ModelComponent(Entity* pOwner) : PARENT(pOwner)
{
	mp_ModelData = nullptr;
	m_modelScale = 0.0f;
}

void ModelComponent::Draw(Shader* a_shader)
{
	//Null Check
	if (!a_shader)
	{
		return;
	}

	if (!mp_ModelData)
	{
		return;
	}

	//Get Transform Component
	TransformComponent* pTransformComponent = static_cast<TransformComponent*>(m_ownerEntity->FindComponentOfType(TRANSFORM));
	if (!pTransformComponent)
	{
		return;
	}

	glm::mat4 modelMatrix = pTransformComponent->GetEntityMatrix();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(m_modelScale, m_modelScale, m_modelScale));
	a_shader->setMat4("model", modelMatrix);
	mp_ModelData->Draw(*a_shader);
}