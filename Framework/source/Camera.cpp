#include "Camera.h"
#include "InputManager.h"


CCamera::CCamera()
{
    m_projectionMatrix = glm::mat4(1.f);
    m_transform = glm::mat4(1.f);
}

CCamera::~CCamera() {}

void CCamera::SetPosition(const glm::vec3& a_v3Pos)
{
    m_transform[3] = glm::vec4(a_v3Pos, 1.f);
}

const glm::vec3& CCamera::GetPosition() const
{
    return  m_transform[3].xyz;
}

void CCamera::SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far)
{
    m_aspectRatio = a_aspectRatio;
    m_fov = a_fieldOfView;
    m_zNear = a_near;
    m_zFar = a_far;
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
}

void CCamera::SetOrthographic(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far)
{
    m_aspectRatio = (a_right - a_left) / (a_bottom - a_top);
    m_zNear = a_near;
    m_zFar = a_far;
    m_projectionMatrix = glm::ortho(a_left, a_right, a_bottom, a_top, m_zNear, m_zFar);
}

void CCamera::LookAt(const glm::vec3& a_v3Target, const glm::vec3& a_up)
{
    glm::vec3 pos = m_transform[3].xyz;
    glm::lookAt(pos, glm::normalize(a_v3Target - pos), a_up);
}

glm::vec3 CCamera::ScreenToRay(const glm::vec2& a_screenCoord) const
{
    glm::mat4 projectionViewMat4 = m_projectionMatrix * glm::inverse(m_transform);
    glm::mat4 inversePV = glm::inverse(projectionViewMat4);
    glm::vec4 nearPlanePosSpaceCoords = inversePV * glm::vec4(a_screenCoord.x, a_screenCoord.y, -1.f, 1.f);
    nearPlanePosSpaceCoords /= nearPlanePosSpaceCoords.w;
    glm::vec3 v3ProjectedRay = glm::normalize(nearPlanePosSpaceCoords.xyz - m_transform[3].xyz);
    return v3ProjectedRay;
}

void CCamera::Update(float a_deltaTime)
{
    InputManager* im = InputManager::GetInstance();
    if (im != nullptr)
    {
        CCamera::Movement movement = (CCamera::Movement)(0);

        ZoomFOV(im->GetMouseScrollYOffset());
        uint16_t yawButt = m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::YAW)))];
        uint16_t pitchButt = m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::PITCH)))];

        float xOffset = 0.f; float yOffset = 0.f;
        if (im->GetMouseButtonState(yawButt) | im->GetMouseButtonState(pitchButt))
        {
            im->GetMouseOffsets(xOffset, yOffset);
            movement = movement | CCamera::Movement::PITCH | CCamera::Movement::YAW;
            ProcessMovement(CCamera::Movement::PITCH | CCamera::Movement::YAW, yOffset, xOffset, 1.f, a_deltaTime);
        }
        float speedModifier = 1.f;
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::FORWARD)))]))) {
            movement = movement | CCamera::Movement::FORWARD;
        }
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::BACKWARD)))]))) {
            movement = movement | CCamera::Movement::BACKWARD;
        }
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::LEFT)))]))) {
            movement = movement | CCamera::Movement::LEFT;
        }
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::RIGHT)))]))) {
            movement = movement | CCamera::Movement::RIGHT;
        }
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::UP)))]))) {
            movement = movement | CCamera::Movement::UP;
        }
        if ((im->GetKeyState(m_movementKeyValues[(uint16_t)(log2((float)(CCamera::Movement::DOWN)))]))) {
            movement = movement | CCamera::Movement::DOWN;
        }

        ProcessMovement(movement, yOffset, xOffset, speedModifier, a_deltaTime);
    }   
}

void CCamera::SetMovementKeyValue(Movement a_movement, uint16_t a_keyValue)
{
    uint16_t exponent = log2((float)(a_movement));
    if (exponent < 8) //8 is number of movement paramaters in the array
    {
        m_movementKeyValues[exponent] = a_keyValue; 
    }
}

void CCamera::ProcessMovement(CCamera::Movement a_movement, float a_pitch, float a_yaw, float a_speedModifier, float a_deltaTime,  const glm::vec3& a_up)
{
    static float speed = 2.f;
    
    glm::vec4 vForward = m_transform[2];
    glm::vec4 vRight = m_transform[0];
    glm::vec4 vUp = m_transform[1];
    glm::vec4 vTranslation = m_transform[3];
    glm::mat4 rotationMatrix;

    //Camera Movement
    float frameSpeed = speed * a_speedModifier * a_deltaTime;

    if (a_movement & CCamera::FORWARD)
    {
        vTranslation -= vForward * frameSpeed;
    }
    if (a_movement & CCamera::BACKWARD)
    {
        vTranslation += vForward * frameSpeed;
    }
    if (a_movement & CCamera::LEFT)
    {
        vTranslation -= vRight * frameSpeed;
    }
    if (a_movement & CCamera::RIGHT)
    {
        vTranslation += vRight * frameSpeed;
    }
    if (a_movement & CCamera::UP)
    {
        vTranslation += vUp * frameSpeed;
    }
    if (a_movement & CCamera::DOWN)
    {
        vTranslation -= vUp * frameSpeed;
    }
    m_transform[3] = vTranslation;

    //Handle Camera Rotation
    if (a_movement & CCamera::PITCH)
    {
        rotationMatrix = glm::axisAngleMatrix(vRight.xyz(), a_pitch / 150.f);
        vRight = rotationMatrix * vRight;
        vUp = rotationMatrix * vUp;
        vForward = rotationMatrix * vForward;
    }
    if (a_movement & CCamera::YAW)
    {
        rotationMatrix = glm::axisAngleMatrix(-a_up.xyz(), a_yaw / 150.f);
        vRight = rotationMatrix * vRight;
        vUp = rotationMatrix * vUp;
        vForward = rotationMatrix * vForward;
    }
    m_transform[0] = vRight;
    m_transform[1] = vUp;
    m_transform[2] = vForward;
}

void CCamera::ZoomFOV(float a_fovDelta)
{
    if (m_fov > 1.f && m_fov < 120.f)
    {
        m_fov -= a_fovDelta;
    }
    SetPerspective(m_fov, m_aspectRatio, m_zNear, m_zFar);
}
