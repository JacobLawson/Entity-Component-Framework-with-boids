#include "MainScene.h"

#include <framework.h>
#include <learnopengl/include/shader.h>
#include <learnopengl/include/model.h>

//Project Includes
#include <MainScene.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <ModelComponent.h>
#include <CameraComponent.h>
#include <FreeCameraComponent.h>
#include <BrainComponent.h>
#include <imgui.h>

void print(int n, const std::string& str) {
	std::string msg = std::to_string(n) + " : " + str;
	std::cout << msg << std::endl;
}



void MainScene::Initialise()
{
	// build and compile shaders
	AddShader("MainShader", new Shader("shaders/model_loading.vs", "shaders/model_loading.fs"));

	//Initialise variables
	m_numberOfBoids = 1;

	m_wanderForceSetting = 1.f;
	m_cohesionForceSetting = 1.f;
	m_alignmentForceSetting = 1.f;
	m_seperationForceSetting = 1.f;
	m_boidSpeed = 1.0f;
	m_neighbourhoodRadius = 5.0f;
	m_wanderJitter = 0.5f;

	//Setup Camera
	AddEntity("MainCamera", new Entity());
	TransformComponent* tc = new TransformComponent(GetEntity("MainCamera"));
	tc->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(0.0f, 1.2f, 4.0f));
	GetEntity("MainCamera")->AddComponent(tc);
	CameraComponent* cc = new CameraComponent(GetEntity("MainCamera"));
	cc->GetFramework(UseFramework());
	cc->LookAt(glm::vec3(0.f, 0.f, 0.f));
	GetEntity("MainCamera")->AddComponent(cc);
	FreeCameraComponent* fcc = new FreeCameraComponent(GetEntity("MainCamera"));
	GetEntity("MainCamera")->AddComponent(fcc);

	//Seed the random
	srand(time(nullptr));

	//Create Entities
	CreateBoidEntity();
}

void MainScene::Update()
{
	GetShader("MainShader")->use();
	glm::mat4 projection;
	glm::mat4 view;
	CameraComponent* camera = static_cast<CameraComponent*>(GetEntity("MainCamera")->FindComponentOfType(CAMERA));
	camera->GetCurrentProjectionView(projection, view);
	GetShader("MainShader")->setMat4("projection", projection);
	GetShader("MainShader")->setMat4("view", view);

	std::map<const unsigned int, Entity*>::const_iterator xIter;
	UpdateBoids(xIter);
	
	//Using IMGUI to control the simulation
	UseFramework()->ImguiGetNewFrame();
	ImGui::Begin("Demo Window");
	{
		ImGui::Text("Create new Boid / Obstacle");
		static int i0 = 25;
		ImGui::InputInt("input int", &i0);
		if (ImGui::Button("Create Boid", ImVec2(120, 30)))			//Create boid entites
		{
			for (int i = 0; i < i0; i++)
			{
				CreateBoidEntity();
			}
		}
		if (ImGui::Button("Create Obstacle", ImVec2(120, 30)))	//Create Obsticales that the boids try to avoid
		{
			for (int i = 0; i < i0; i++)
			{
				CreateCube();
			}
		}

		//Adjust the flocking parameters used in the simulation
		ImGui::Text("Adjust Flocking Parameters");
		static float SVal = 1.f;
		static float AVal = 1.f;
		static float CVal = 1.f;
		static float WVal = 1.f;
		if (ImGui::InputFloat("Seperation Force", &SVal))
		{
			m_seperationForceSetting = SVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetSeperationForce(m_seperationForceSetting);
				}
			}
			
		}
		if (ImGui::InputFloat("Alignment Force", &AVal))
		{
			m_alignmentForceSetting = AVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetAlignmentForce(m_alignmentForceSetting);
				}
			}
		}
		if (ImGui::InputFloat("Cohesion Force", &CVal))
		{
			m_cohesionForceSetting = CVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetCohesionForce(m_cohesionForceSetting);
				}
			}
		}
		if (ImGui::InputFloat("Wander Force", &WVal))
		{
			m_wanderForceSetting = WVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetWanderForce(m_wanderForceSetting);
				}
			}
		}
		
		ImGui::Text("Adjust Steering Parameters");
		static float speedVal = 1.0f;
		static float JitterVal = 0.5f;
		static float neighborhoodVal = 5.0f;

		if (ImGui::InputFloat("Speed", &speedVal))
		{
			m_boidSpeed = speedVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetMoveSpeed(m_boidSpeed);
				}
			}
		}
		if (ImGui::InputFloat("Neighbourhood Radius", &neighborhoodVal))
		{
			m_neighbourhoodRadius = neighborhoodVal;
			for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
			{
				Entity* entityToUpdate = xIter->second;
				BrainComponent* bc = static_cast<BrainComponent*>(entityToUpdate->FindComponentOfType(BRAIN));
				if (bc != nullptr)
				{
					bc->SetNeighbourhoodRadius(m_neighbourhoodRadius);
				}
			}
		}


	} ImGui::End();
}

bool MainScene::UpdateBoids(std::map<const unsigned int, Entity*>::const_iterator a_xIter)
{
	if (m_entitesUpdatecounter > 2)
	{
		m_entitesUpdatecounter = 0;
	}
	//Update Boids
	int size = Entity::GetEntityList().size() / 2;
	if (m_entitesUpdatecounter % 2 == 0)	//first half to be run
	{

		for (a_xIter = Entity::GetEntityList().begin(); a_xIter != Entity::GetEntityList().end(); a_xIter++)
		{
			Entity* entityToUpdate = a_xIter->second;
			if (entityToUpdate)
			{
				entityToUpdate->Update(UseFramework()->GetDeltaTime());
				if (a_xIter->first > size)
				{
					m_entitesUpdatecounter += 1;
					return true;
				}
			}
		}
	}
	else
	{
		for (a_xIter = Entity::GetEntityList().begin(); a_xIter != Entity::GetEntityList().end(); a_xIter++)
		{
			if (a_xIter->first > size)
			{
				Entity* entityToUpdate = a_xIter->second;
				if (entityToUpdate)
				{
					entityToUpdate->Update(UseFramework()->GetDeltaTime());
				}
			}
			else
			{
				a_xIter++;
			}
		}
		m_entitesUpdatecounter += 1;
		return true;
	}
	return false;	
}	


void MainScene::Draw()
{
	std::map<const unsigned int, Entity*>::const_iterator xIter;
	for (xIter = Entity::GetEntityList().begin(); xIter != Entity::GetEntityList().end(); xIter++)
	{
		Entity* entityToDraw = xIter->second;
		if (entityToDraw)
		{
			entityToDraw->Draw(GetShader("MainShader"));
		}
	}
}

void MainScene::End()
{
	
}

void MainScene::CreateBoidEntity()
{
	Entity* temp = new Entity();
	//Transform component
	TransformComponent* pTransformComponent = new TransformComponent(temp);
	pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(UseFramework()->RandomRange(-5.f, 5.f), UseFramework()->RandomRange(-5.f, 5.f), UseFramework()->RandomRange(-5.f, 5.f)));
	temp->AddComponent(pTransformComponent);
	//Model component
	ModelComponent* pModelComponent = new ModelComponent(temp);
	pModelComponent->SetModel(new Model("models/nanosuit/nanosuit.obj"));
	pModelComponent->SetScale(0.2f);
	temp->AddComponent(pModelComponent);
	//BrainComponent
	BrainComponent* pBrain = new BrainComponent(temp);

	//Set Simulation Settings for the boid
	pBrain->SetMoveSpeed(m_boidSpeed);
	pBrain->SetWanderJitter(m_wanderJitter);
	pBrain->SetNeighbourhoodRadius(m_neighbourhoodRadius);
	pBrain->SetSeperationForce(m_seperationForceSetting);
	pBrain->SetAlignmentForce(m_alignmentForceSetting);
	pBrain->SetCohesionForce(m_cohesionForceSetting);
	pBrain->SetWanderForce(m_wanderForceSetting);
	temp->AddComponent(pBrain);
	
	m_numberOfBoids += 1;
	temp = nullptr;
}

void MainScene::CreateCube()
{
	Entity* temp = new Entity();
	//Transform component
	TransformComponent* pTransformComponent = new TransformComponent(temp);
	pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(UseFramework()->RandomRange(-5.f, 5.f), UseFramework()->RandomRange(-5.f, 5.f), UseFramework()->RandomRange(-5.f, 5.f)));
	temp->AddComponent(pTransformComponent);
	//Model component
	ModelComponent* pModelComponent = new ModelComponent(temp);
	pModelComponent->SetModel(new Model("models/nanosuit/nanosuit.obj"));
	pModelComponent->SetScale(0.1f);
	temp->AddComponent(pModelComponent);
}
