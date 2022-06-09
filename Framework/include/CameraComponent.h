#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Framework;
class TransformComponent;

class CameraComponent : public Component
{
public:
	CameraComponent(Entity* pOwner);
	~CameraComponent();

	void Update(float a_deltaTime);
	void Draw(Shader* pShader) {};

	void SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far);
	void SetOrthographic(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far);

	void LookAt(const glm::vec3& a_v3Target, const glm::vec3& a_up = glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 ScreenToRay(const glm::vec2& a_screenCoord);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void GetCurrentProjectionView(glm::mat4& a_projection, glm::mat4& a_view);
	void GetFramework(Framework* a_framework);

	float GetFOV() { return m_fov; }
	void SetFOV(float a_fov) { m_fov = a_fov; }
	float GetAspectRatio() { return m_aspectRatio; }
	void ZGet(float a_near, float a_far) { a_near = m_zNear; a_far = m_zFar; }

	TransformComponent* GetTransformComponent();

private:
	Framework* m_pFramework;

	glm::mat4 m_projectionMatrix;
	float m_aspectRatio;
	float m_fov;
	float m_zNear;
	float m_zFar;

	float m_lastX;
	float m_lastY;
	bool m_firstMouse = true;
};
