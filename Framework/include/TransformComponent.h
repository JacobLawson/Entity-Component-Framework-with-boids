#pragma once
//includes
#include "Component.h"
#include <glm/ext.hpp>

enum MATRIX_ROW
{
	RIGHT_VECTOR,
	UP_VECTOR,
	FORWARD_VECTOR,
	POSITION_VECTOR
};

class TransformComponent : public Component
{
public:
	TransformComponent(Entity* pOwner);
	~TransformComponent();

	virtual void Update(float a_deltaTime) {};
	virtual void Draw(Shader* pShader) {};

	const glm::mat4& GetEntityMatrix() { return m_EntityMatrix_M4; }

	void SetEntityMatrixRow(MATRIX_ROW a_row, glm::vec3 a_vec3);
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW a_row);

private:

	glm::mat4 m_EntityMatrix_M4;
};