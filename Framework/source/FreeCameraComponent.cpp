#include "FreeCameraComponent.h"
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include <Entity.h>

#include <iostream>

typedef Component PARENT;

FreeCameraComponent::FreeCameraComponent(Entity* pOwner) : PARENT(pOwner)
{
	m_componentType = FREE_CAMERA;

    SetMovementKeyValue(Movement::FORWARD, GLFW_KEY_W);
    SetMovementKeyValue(Movement::LEFT, GLFW_KEY_A);
    SetMovementKeyValue(Movement::BACKWARD, GLFW_KEY_S);
    SetMovementKeyValue(Movement::RIGHT, GLFW_KEY_D);
    SetMovementKeyValue(Movement::UP, GLFW_KEY_Q);
    SetMovementKeyValue(Movement::DOWN, GLFW_KEY_E);
    SetMovementKeyValue(Movement::PITCH, GLFW_MOUSE_BUTTON_2);
    SetMovementKeyValue(Movement::YAW, GLFW_MOUSE_BUTTON_2);

    p_myCamera = static_cast<CameraComponent*>(m_ownerEntity->FindComponentOfType(CAMERA));
    p_myTransform = static_cast<TransformComponent*>(m_ownerEntity->FindComponentOfType(TRANSFORM));
    p_inputManager = InputManager::GetInstance();
}

void FreeCameraComponent::SetMovementKeyValue(Movement a_movement, uint16_t a_keyValue)
{
    uint16_t exponent = log2((float)(a_movement));
    if (exponent < 8) //8 is number of movement paramaters in the array
    {
        m_movementKeyValues[exponent] = a_keyValue;
    }
}

void FreeCameraComponent::ProcessMovement(Movement a_movement, float a_pitch, float a_yaw, float a_speedModifier, float a_deltaTime, const glm::vec3& a_up)
{
    float speed = 2.f;

    glm::vec4 vForward = p_myTransform->GetEntityMatrix()[2];
    glm::vec4 vRight = p_myTransform->GetEntityMatrix()[0]; //m_transform[0];
    glm::vec4 vUp = p_myTransform->GetEntityMatrix()[1]; //m_transform[1];
    glm::vec4 vTranslation = p_myTransform->GetEntityMatrix()[3];//m_transform[3];
    glm::mat4 rotationMatrix;

    //Camera Movement
    float frameSpeed = speed * a_speedModifier * a_deltaTime;

    if (a_movement & FORWARD)
    {
        vTranslation -= vForward * frameSpeed;
    }
    if (a_movement & BACKWARD)
    {
        vTranslation += vForward * frameSpeed;
    }
    if (a_movement & LEFT)
    {
        vTranslation -= vRight * frameSpeed;
    }
    if (a_movement & RIGHT)
    {
        vTranslation += vRight * frameSpeed;
    }
    if (a_movement & UP)
    {
        vTranslation += vUp * frameSpeed;
    }
    if (a_movement & DOWN)
    {
        vTranslation -= vUp * frameSpeed;
    }
    p_myTransform->SetEntityMatrixRow(POSITION_VECTOR, vTranslation);
    rotationMatrix = glm::mat4(1.f);
    //Handle Camera Rotation
    if (a_movement & PITCH)
    {
        std::cout << "m_pitch: " << a_pitch << " deltaTime: " << a_deltaTime << std::endl;
        rotationMatrix = glm::axisAngleMatrix(vRight.xyz(), a_pitch * a_deltaTime);
        vRight = rotationMatrix * vRight;
        vUp = rotationMatrix * vUp;
        vForward = rotationMatrix * vForward;
    }
    if (a_movement & YAW)
    {
        rotationMatrix = glm::axisAngleMatrix(a_up.xyz(), a_yaw * a_deltaTime);
        vRight = rotationMatrix * vRight;
        vUp = rotationMatrix * vUp;
        vForward = rotationMatrix * vForward;
    }
    p_myTransform->SetEntityMatrixRow(RIGHT_VECTOR, vRight);
    p_myTransform->SetEntityMatrixRow(UP_VECTOR, vUp);
    p_myTransform->SetEntityMatrixRow(FORWARD_VECTOR, vForward);
}

void FreeCameraComponent::Update(float a_deltaTime)
{
    if (p_inputManager != nullptr && p_myCamera != nullptr)
    {
        Movement movement = (Movement)(0);

        ZoomFOV(p_inputManager->GetMouseScrollYOffset());
        uint16_t yawButt = m_movementKeyValues[(uint16_t)(log2((float)(Movement::YAW)))];
        uint16_t pitchButt = m_movementKeyValues[(uint16_t)(log2((float)(Movement::PITCH)))];

        float xOffset = 0.f; float yOffset = 0.f;
        if (p_inputManager->GetMouseButtonState(yawButt) || p_inputManager->GetMouseButtonState(pitchButt))
        {
            p_inputManager->GetMouseOffsets(xOffset, yOffset);
            movement = movement | Movement::PITCH | Movement::YAW;
            //ProcessMovement(movement, yOffset, xOffset, 1.f, a_deltaTime);
        }
        float speedModifier = 1.f;
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::FORWARD)))]))) {
            movement = movement | Movement::FORWARD;
        }
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::BACKWARD)))]))) {
            movement = movement | Movement::BACKWARD;
        }
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::LEFT)))]))) {
            movement = movement | Movement::LEFT;
        }
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::RIGHT)))]))) {
            movement = movement | Movement::RIGHT;
        }
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::UP)))]))) {
            movement = movement | Movement::UP;
        }
        if ((p_inputManager->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(Movement::DOWN)))]))) {
            movement = movement | Movement::DOWN;
        }

        ProcessMovement(movement, yOffset, xOffset, speedModifier, a_deltaTime);
    }
}

void FreeCameraComponent::ZoomFOV(float a_fovDelta)
{
    float zNear = 0, zFar = 0;
    float fov = p_myCamera->GetFOV();
    if (fov > 1.f && fov < 120.f)
    {   
        p_myCamera->SetFOV(p_myCamera->GetFOV() - a_fovDelta);
    }
}
