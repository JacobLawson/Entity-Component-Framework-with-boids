#include "CameraComponent.h"
#include <framework.h>

#include <Entity.h>
#include <TransformComponent.h>

//typeDefs
typedef Component PARENT; 

CameraComponent::CameraComponent(Entity* pOwner) : PARENT(pOwner)
{
    m_componentType = CAMERA;
    m_aspectRatio = 0.f;
    m_fov = 0.f;
    m_zNear = 0.f;
    m_zFar = 0.f;
}

void CameraComponent::Update(float a_deltaTime)
{
    SetPerspective(m_fov, m_aspectRatio, m_zNear, m_zFar);
}

void CameraComponent::SetPerspective(float a_fieldOfView, float a_aspectRatio, float a_near, float a_far)
{
    m_aspectRatio = a_aspectRatio;
    m_fov = a_fieldOfView;
    m_zNear = a_near;
    m_zFar = a_far;
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_zNear, m_zFar);
}

void CameraComponent::SetOrthographic(float a_left, float a_right, float a_top, float a_bottom, float a_near, float a_far)
{
    m_aspectRatio = (a_right - a_left) / (a_bottom - a_top);
    m_zNear = a_near;
    m_zFar = a_far;
    m_projectionMatrix = glm::ortho(a_left, a_right, a_bottom, a_top, m_zNear, m_zFar);
}

void CameraComponent::LookAt(const glm::vec3& a_v3Target, const glm::vec3& a_up)
{
    glm::vec3 pos = GetTransformComponent()->GetEntityMatrixRow(POSITION_VECTOR).xyz;
    glm::lookAt(pos, glm::normalize(a_v3Target - pos), a_up);
}

glm::vec3 CameraComponent::ScreenToRay(const glm::vec2& a_screenCoord)
{
    glm::mat4 projectionViewMat4 = m_projectionMatrix * glm::inverse(GetTransformComponent()->GetEntityMatrix());
    glm::mat4 inversePV = glm::inverse(projectionViewMat4);
    glm::vec4 nearPlanePosSpaceCoords = inversePV * glm::vec4(a_screenCoord.x, a_screenCoord.y, -1.f, 1.f);
    nearPlanePosSpaceCoords /= nearPlanePosSpaceCoords.w;
    glm::vec3 v3ProjectedRay = glm::normalize(nearPlanePosSpaceCoords.xyz - GetTransformComponent()->GetEntityMatrixRow(POSITION_VECTOR).xyz);
    return v3ProjectedRay;
}

glm::mat4 CameraComponent::GetProjectionMatrix()
{
    return m_projectionMatrix;
}

glm::mat4 CameraComponent::GetViewMatrix()
{
    return glm::inverse(GetTransformComponent()->GetEntityMatrix());
}

void CameraComponent::GetCurrentProjectionView(glm::mat4& a_projection, glm::mat4& a_view)
{
    a_projection = GetProjectionMatrix();
    a_view = GetViewMatrix();
}

void CameraComponent::GetFramework(Framework* a_framework)
{
    m_pFramework = a_framework;
    if (m_pFramework != nullptr)
    {
        m_lastX = m_pFramework->GetWindowWidth() /2.f;
        m_lastY = m_pFramework->GetWindowHeight() /2.f;
        m_firstMouse = true;
        
        //Set Initial Perspective
        SetPerspective(60.f, (float)m_pFramework->GetWindowWidth() / (float)m_pFramework->GetWindowHeight(), 0.1f, 1000.f);
    }
}

TransformComponent* CameraComponent::GetTransformComponent()
{
    //Get Transform Component
    TransformComponent* pTransformComponent = static_cast<TransformComponent*>(m_ownerEntity->FindComponentOfType(TRANSFORM));
    if (pTransformComponent)
    {
        return pTransformComponent;
    }
    return nullptr;
}
