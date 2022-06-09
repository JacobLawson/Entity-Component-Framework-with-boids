#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class InputManager;
class CameraComponent;
class TransformComponent;

class FreeCameraComponent : public Component
{
public:
	typedef enum Movement {
		FORWARD = 0x01,
		BACKWARD = 0x02,
		LEFT = 0x04,
		RIGHT = 0x08,
		UP = 0x10,
		DOWN = 0x20,
		YAW = 0x40,
		PITCH = 0x80
	}Movement;

	FreeCameraComponent(Entity* pOwner);
	~FreeCameraComponent();

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* pShader) {};

	void ZoomFOV(float a_fovDelta);

	void SetMovementKeyValue(Movement a_movement, uint16_t a_keyValue);
	void ProcessMovement(Movement a_movement, float a_pitch, float a_yaw, float a_speedModifier, float a_deltaTime, const glm::vec3& a_up = glm::vec3(0.f, 1.f, 0.f));

	inline friend Movement operator | (Movement a, Movement b)
	{
		return static_cast<Movement>(static_cast<int>(a) | static_cast<int>(b));
	}

private:
	InputManager* p_inputManager;
	CameraComponent* p_myCamera;
	TransformComponent* p_myTransform;

	uint16_t m_movementKeyValues[9];
};