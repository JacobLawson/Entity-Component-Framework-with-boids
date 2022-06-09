#pragma once
#include "Component.h"

class Model;

class ModelComponent : public Component
{
public:
	ModelComponent(Entity* pOwner);
	~ModelComponent();

	virtual void Update(float a_deltaTime) {};
	virtual void Draw(Shader* pShader);

	void SetModel(Model* a_newModel) { mp_ModelData = a_newModel; }
	void SetScale(float a_scale) { m_modelScale = a_scale; }

private:
	Model* mp_ModelData;
	float m_modelScale;
};