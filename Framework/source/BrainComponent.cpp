#include "include\BrainComponent.h"
//Project Includes
#include "Entity.h"
#include "TransformComponent.h"

//Wander Constants
static const float CIRCLE_FORWARD_MULITPLIER = 0.5f;
static const float WANDER_RADIUS = 4.0f;

BrainComponent::BrainComponent(Entity* a_Entity) : Component(a_Entity)
{
	m_ownerEntity = a_Entity;
	m_componentType = BRAIN;
	m_currentVelocity = glm::vec3(0.f);
	m_wanderPoint = glm::vec3(0.f);

	//Initialise Variables
	m_wanderJitter = 0.5f;
	m_moveSpeed = 1.0f;
	m_neighbourhoodRadius = 5.0f;
}

void BrainComponent::Update(float a_deltaTime)
{
	//Get Owner Entity
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) { return; }

	//Get Transform Component
	tc = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!tc) { return; }

	//Get Vectors we need for calculations
	glm::vec3 vForward = tc->GetEntityMatrixRow(FORWARD_VECTOR);
	glm::vec3 vPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);

	//Calculate forces
	glm::vec3 vNewForce(0.f);
	
	//Seek
	//vNewForce = CalculateSeekForce(vPosition * CIRCLE_FORWARD_MULITPLIER, vPosition);
	vNewForce += CalculateForces(m_seperationMultiplier, m_alignmentMultiplier, m_cohesionMultiplier, m_wanderMultiplier);
	//flee from obstacles
	vNewForce += AvoidObstacles();
	//remain within contraints
	vNewForce += ConstraintVolume(25.0f);

	//Apply Force
	m_currentVelocity += vNewForce;
	//clamp Velocity
	m_currentVelocity = glm::clamp(m_currentVelocity, glm::vec3(-10.f, 0.f, -10.f), glm::vec3(10.f, 0.f,10.f));
	//Apply Velocity to Position
	vPosition += m_currentVelocity * a_deltaTime;
	
	vForward = m_currentVelocity;
	//Normalize Forward
	if (glm::length(vForward) > 0.f)
	{
		vForward = glm::normalize(vForward);
	}

	//Up and Right
	glm::vec3 vUp = tc->GetEntityMatrixRow(UP_VECTOR);
	glm::vec3 vRight = glm::cross(vUp, vForward);

	//Update Matrix
	tc->SetEntityMatrixRow(RIGHT_VECTOR, vRight);
	tc->SetEntityMatrixRow(FORWARD_VECTOR, vForward);
	tc->SetEntityMatrixRow(POSITION_VECTOR, vPosition);
}

glm::vec3 BrainComponent::CalculateSeekForce(const glm::vec3& a_targetVector, const glm::vec3& a_positionVector) const
{
	//Calculate Target Direction
	glm::vec3 targetDir(a_targetVector - a_positionVector);
	if (glm::length(targetDir) > 0.f)
	{
		targetDir = glm::normalize(targetDir);
	}
	//Calculate new Velocity
	glm::vec3 newVelocity = targetDir * m_moveSpeed;

	return (newVelocity - m_currentVelocity);
}

glm::vec3 BrainComponent::CalculateFleeForce(const glm::vec3& a_targetVector, const glm::vec3& a_positionVector) const
{
	//Calculate Target Direction
	glm::vec3 targetDir(a_positionVector - a_targetVector);
	if (glm::length(targetDir) > 0.f)
	{
		targetDir = glm::normalize(targetDir);
	}
	//Calculate new Velocity
	glm::vec3 newVelocity = targetDir * m_moveSpeed;

	return (newVelocity - m_currentVelocity);
}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& a_forwardVector, const glm::vec3& a_positionVector)
{
	//Project a point in front of us, for the center of our sphere
	glm::vec3 sphereOriginPoint = a_positionVector + (a_forwardVector * CIRCLE_FORWARD_MULITPLIER);
	
	if (glm::length(m_wanderPoint) == 0.f)
	{
		//Find a Random Point on a sphere
		glm::vec3 randomPointOnSphere = glm::sphericalRand(WANDER_RADIUS);
		
		//Add the random point to origin point
		m_wanderPoint = sphereOriginPoint + randomPointOnSphere;
	}
	//Calculate Direction to move to
	glm::vec3 directionToTarget = glm::normalize(m_wanderPoint - sphereOriginPoint) * WANDER_RADIUS;

	//Find our final target point
	m_wanderPoint = sphereOriginPoint + directionToTarget;
	//Add Jitter
	m_wanderPoint += glm::sphericalRand(m_wanderJitter);
	
	return CalculateSeekForce(m_wanderPoint, a_positionVector);
}

glm::vec3 BrainComponent::CalculateSeperationForce()
{
	//Final Seperation Vector
	glm::vec3 seperationVelocity(0.f);
	unsigned int uNeighbourCount = 0;

	//Get Transform Component
	TransformComponent* tc = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
	if (tc == nullptr)
	{
		//Entity has no transform Component
		return glm::vec3(0.f);
	}
	
	//Get the position
	glm::vec3 localPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);

	//Create an iterator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
	std::map<const unsigned int, Entity*>::const_iterator xConstItter;
	//Loop over all entities in scene
	for (xConstItter = xEntityMap.begin(); xConstItter != xEntityMap.end(); ++xConstItter)
	{
		//Get Target Entity
		Entity* pTarget = xConstItter->second;

		//Make sure we are not checking against ourselves
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			//Null Check target entity
			if (pTarget != nullptr)
			{
				//Get Components from target
				TransformComponent* targetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
				if (targetTransform != nullptr)
				{
					BrainComponent* targetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));
					//Make sure we are only checking for other boid objects with a brain
					if (targetBrain != nullptr)
					{
						//Find Distance between Target and ourself
						glm::vec3 targetPosition = targetTransform->GetEntityMatrixRow(POSITION_VECTOR);
						float distanceBetween = glm::length(targetPosition - localPosition);

						//Check distance is within our neighbourhood;
						if (distanceBetween < m_neighbourhoodRadius)
						{
							seperationVelocity += (localPosition - targetPosition);
							uNeighbourCount++;
						}

						if (glm::length(seperationVelocity) > 0.f)
						{
							seperationVelocity /= uNeighbourCount;
							seperationVelocity = glm::normalize(seperationVelocity);
						}
					}
				}
			}
		}
	}
	return seperationVelocity;
}

glm::vec3 BrainComponent::AlignmentForce()
{
	//Final Alignment Vector
	glm::vec3 alignmentVelocity(0.f);
	unsigned int uNeighbourCount = 0;

	//Get Transform Component
	TransformComponent* tc = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
	if (tc == nullptr)
	{
		//Entity has no transform Component
		return glm::vec3(0.f);
	}

	//Get the position
	glm::vec3 localPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);

	//Create an iterator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
	std::map<const unsigned int, Entity*>::const_iterator xConstItter;
	//Loop over all entities in scene
	for (xConstItter = xEntityMap.begin(); xConstItter != xEntityMap.end(); ++xConstItter)
	{

		//Get Target Entity
		Entity* pTarget = xConstItter->second;
		
		//Make sure we are not checking against ourselves
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			//Null Check target entity
			if (pTarget != nullptr)
			{
				//Get Target Components
				TransformComponent* targetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
				if (targetTransform != nullptr)
				{
					//Make sure we are only checking for other boid objects with a brain
					BrainComponent* targetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));
					if (targetBrain != nullptr)
					{

						//Find Distance between Target and ourself
						glm::vec3 targetPosition = targetTransform->GetEntityMatrixRow(POSITION_VECTOR);
						float distanceBetween = glm::length(targetPosition - localPosition);

						//Check distance is within our neighbourhood;
						if (distanceBetween < m_neighbourhoodRadius)
						{
							alignmentVelocity += targetBrain->GetCurrentVelocity(); // + //(CalculateFleeForce(localPosition * CIRCLE_FORWARD_MULITPLIER, targetPosition) / 2.0f);
							uNeighbourCount++;
						}

						if (glm::length(alignmentVelocity) > 0.f)
						{
							alignmentVelocity /= uNeighbourCount;
							alignmentVelocity = glm::normalize(alignmentVelocity);
						}
					}

					
				}
			}
		}
	}
	return alignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionForce()
{
	//Final Alignment Vector
	glm::vec3 cohesionVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//Get Transform Component
	TransformComponent* tc = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
	if (tc == nullptr)
	{
		//Entity has no transform Component
		return glm::vec3(0.f);
	}

	//Get the position
	glm::vec3 localPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);

	//Create an iterator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
	std::map<const unsigned int, Entity*>::const_iterator xConstItter;
	//Loop over all entities in scene
	for (xConstItter = xEntityMap.begin(); xConstItter != xEntityMap.end(); ++xConstItter)
	{
		//Get Target Entity
		Entity* pTarget = xConstItter->second;

		//Make sure we are not checking against ourselves
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			//Null Check target entity
			if (pTarget != nullptr)
			{
				//Get Target Components
				TransformComponent* targetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
				if (targetTransform != nullptr)
				{
					//Make sure we are only checking for other boid objects with a brain
					BrainComponent* targetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));
					if (targetBrain != nullptr)
					{
						//Find Distance between Target and ourself
						glm::vec3 targetPosition = targetTransform->GetEntityMatrixRow(POSITION_VECTOR);
						float distanceBetween = glm::length(targetPosition - localPosition);

						//Check distance is within our neighbourhood;
						if (distanceBetween < m_neighbourhoodRadius)
						{
							cohesionVelocity += targetPosition;
							uNeighbourCount++;
						}

						if (glm::length(cohesionVelocity) > 0.f)
						{
							cohesionVelocity /= uNeighbourCount;
							cohesionVelocity = glm::normalize(cohesionVelocity - localPosition) + (CalculateSeekForce(targetPosition * CIRCLE_FORWARD_MULITPLIER, localPosition));
						}
					}					
				}
			}
		}		
	}
	return cohesionVelocity;
}

glm::vec3 BrainComponent::CalculateForces(float a_seperationStrength, float a_alignmentStrength, float a_cohesionStrength, float a_wanderStrength)
{
	glm::vec3 finalForce(0.f);
	glm::vec3 seperationForce = CalculateSeperationForce() * a_seperationStrength;
	glm::vec3 alignmentForce = AlignmentForce() * a_alignmentStrength;
	glm::vec3 cohesionForce = CalculateCohesionForce() * a_cohesionStrength;
	glm::vec3 wanderForce = CalculateWanderForce(tc->GetEntityMatrixRow(FORWARD_VECTOR) * CIRCLE_FORWARD_MULITPLIER, tc->GetEntityMatrixRow(POSITION_VECTOR)) * a_wanderStrength;

	finalForce = seperationForce + alignmentForce + cohesionForce + wanderForce;
	return finalForce;
}

glm::vec3 BrainComponent::AvoidObstacles()
{
	//Create an iterator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityList();
	std::map<const unsigned int, Entity*>::const_iterator xConstItter;
	for (xConstItter = xEntityMap.begin(); xConstItter != xEntityMap.end(); ++xConstItter)
	{
		//Get our position
		TransformComponent* tc = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
		glm::vec3 localPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);

		//Get Target Entity and their components
		Entity* pTarget = xConstItter->second;
		BrainComponent* targetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));
		TransformComponent* targetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));

		//Only avoid things without a brain (for this program we will just consider all brainless entites as obsticales)
		if (targetBrain == nullptr && targetTransform != nullptr)
		{
			//Find Distance between Target and ourself
			glm::vec3 targetPosition = targetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float distanceBetween = glm::length(targetPosition - localPosition);

			//Check distance is within our neighbourhood;
			if (distanceBetween < m_neighbourhoodRadius)
			{
				return CalculateFleeForce(targetPosition * CIRCLE_FORWARD_MULITPLIER, localPosition);
			}
			return glm::vec3(0.0f);
		}
	}
}

glm::vec3 BrainComponent::ConstraintVolume(float a_size)
{
	//Constrain the boid entity within a bound size
	//Get Component
	TransformComponent* tc = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));
	glm::vec3 localPosition = tc->GetEntityMatrixRow(POSITION_VECTOR);
	//Check we have a transform
	if (tc == nullptr)
	{
		return glm::vec3(0.f);
	}

	//Check if we are outside the defined contraints
	if (localPosition.x >= a_size || localPosition.y >= a_size || localPosition.z >= a_size ||
		localPosition.x <= -a_size || localPosition.y <= -a_size || localPosition.z <= -a_size)
	{
		//Move towards center of the world space
		return CalculateSeekForce(glm::vec3(0.0f), localPosition);
	}
	return glm::vec3(0.0f);
}

