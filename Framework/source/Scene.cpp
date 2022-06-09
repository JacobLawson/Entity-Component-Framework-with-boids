#include "Scene.h"
#include <Framework.h>

#include <learnopengl/include/shader.h>
#include <learnopengl/include/model.h>

//Project Includes
#include <Entity.h>
#include <Component.h>
#include <TransformComponent.h>
#include <ModelComponent.h>

Scene::Scene(std::string a_sceneName, Framework* a_frameworkPointer)
{
	m_name = a_sceneName;
	p_framework = a_frameworkPointer;
}

Scene::~Scene()
{
}

Entity* Scene::GetEntity(std::string a_name)
{
	return m_SceneEntities.find(a_name)->second;
}

Shader* Scene::GetShader(std::string a_name)
{
	return m_SceneShaders.find(a_name)->second;
}

void Scene::AddEntity(string a_name, Entity* a_entity)
{
	m_SceneEntities.emplace(a_name, a_entity);
}

void Scene::AddShader(std::string a_name, Shader* a_shader)
{
	m_SceneShaders.emplace(a_name, a_shader);
}


