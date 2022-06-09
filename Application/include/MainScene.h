#pragma once
#include "Scene.h"

class MainScene : public Scene
{
public:
	using Scene::Scene;
	~MainScene();

	void Initialise();
	void Update();
	bool UpdateBoids(std::map<const unsigned int, Entity*>::const_iterator a_xIter);
	void Draw();
	void End();

	void CreateBoidEntity();
	void CreateCube();

private:
	int m_numberOfBoids;
	int m_entitesUpdatecounter;
	int m_maxNumberOfBoids;

	//Boid Simulation Settings
	float m_wanderForceSetting;
	float m_cohesionForceSetting;
	float m_alignmentForceSetting;
	float m_seperationForceSetting;

	float m_wanderJitter;
	float m_boidSpeed;
	float m_neighbourhoodRadius;
};
