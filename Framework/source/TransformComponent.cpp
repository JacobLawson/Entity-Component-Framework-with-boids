#include "TransformComponent.h"

//TypeDefs
typedef Component PARENT;

TransformComponent::TransformComponent(Entity* pOwner) : PARENT(pOwner)
{
	m_EntityMatrix_M4 = glm::mat4(1.0f);
	m_componentType = TRANSFORM;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetEntityMatrixRow(MATRIX_ROW a_row, glm::vec3 a_vec3)
{
	m_EntityMatrix_M4[a_row] = glm::vec4(a_vec3, (a_row == POSITION_VECTOR ? 1.f : 0.0f));	
}

glm::vec3 TransformComponent::GetEntityMatrixRow(MATRIX_ROW a_row)
{
	return m_EntityMatrix_M4[a_row];
}
