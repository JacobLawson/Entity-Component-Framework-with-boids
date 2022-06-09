#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void SetPosition(const glm::vec3& a_v3Pos);
	const glm::vec3& GetPosition() const;

	void SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far);
	void SetOrthographic(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far);

	void LookAt(const glm::vec3& a_v3Target, const glm::vec3& a_up = glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 ScreenToRay(const glm::vec2& a_screenCoord) const;

	glm::mat4 GetTransform() { return m_transform; }
	glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
	glm::mat4 GetViewMatrix() { return glm::inverse(m_transform); }
	
	typedef enum Movement {
		FORWARD =	0x01,
		BACKWARD =	0x02,
		LEFT =		0x04,
		RIGHT =		0x08,
		UP =		0x10,
		DOWN =		0x20,
		YAW =		0x40,
		PITCH =		0x80
	}Movement;

	inline friend CCamera::Movement operator | (CCamera::Movement a, CCamera::Movement b) 
	{ 
		return static_cast<CCamera::Movement>(static_cast<int>(a) | static_cast<int>(b));
	}

	void Update(float a_deltaTime);  
	void SetMovementKeyValue(Movement a_movement, uint16_t a_keyValue);
	void ProcessMovement(CCamera::Movement a_movement, float a_pitch, float a_yaw, float a_speedModifier, float a_deltaTime, const glm::vec3& a_up = glm::vec3(0.f, 1.f, 0.f));
	void ZoomFOV(float a_fovDelta);


private:
	uint16_t m_movementKeyValues[9];
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_transform;
	float m_aspectRatio;
	float m_fov;
	float m_zNear;
	float m_zFar;
};