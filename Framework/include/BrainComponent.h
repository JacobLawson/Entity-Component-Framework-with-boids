#pragma once

#include "Component.h"
#include <glm/glm.hpp>

class Entity;
class TransformComponent;

class BrainComponent : public Component
{
public:
	BrainComponent(Entity* a_Entity);

	void Start();

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* a_pShader) {};

	glm::vec3 GetCurrentVelocity() const { return m_currentVelocity; }

	//Setters
	void SetMoveSpeed(float a_newSpeed) { m_moveSpeed = a_newSpeed; }
	void SetNeighbourhoodRadius(float a_newRadius) { m_neighbourhoodRadius = a_newRadius; }
	void SetWanderJitter(float a_newJitterValue) { m_wanderJitter = a_newJitterValue; }
	//Set Steering variables
	void SetWanderForce(float a_newForce) { m_wanderMultiplier = a_newForce; }
	void SetCohesionForce(float a_newForce) { m_cohesionMultiplier = a_newForce; }
	void SetAlignmentForce(float a_newForce) { m_alignmentMultiplier = a_newForce; }
	void SetSeperationForce(float a_newForce) { m_seperationMultiplier = a_newForce; }

private:
	//Steering Behaviours
	glm::vec3 CalculateSeekForce(const glm::vec3& a_targetVector, const glm::vec3& a_positionVector) const;
	glm::vec3 CalculateFleeForce(const glm::vec3& a_targetVector, const glm::vec3& a_positionVector) const;
	glm::vec3 CalculateWanderForce(const glm::vec3& a_forwardVector, const glm::vec3& a_positionVector);

	//Flocking Behaviours
	glm::vec3 CalculateSeperationForce();
	glm::vec3 AlignmentForce();
	glm::vec3 CalculateCohesionForce();

	glm::vec3 CalculateForces(float a_seperationStrength, float a_alignmentStrength, float a_cohesionStrength, float a_wanderStrength);

	glm::vec3  AvoidObstacles();

	glm::vec3 ConstraintVolume(float a_size);

	float m_speed;
	float m_wanderRadius;
	glm::vec3 m_currentVelocity;
	glm::vec3 m_wanderPoint;

	//Variables
	float m_moveSpeed;
	float m_neighbourhoodRadius;
	float m_wanderJitter;

	//Steering variables
	float m_wanderMultiplier;
	float m_cohesionMultiplier;
	float m_alignmentMultiplier;
	float m_seperationMultiplier;

	//object Component pointers
	TransformComponent* tc;
};