#pragma once
#include <string>
#include <vector>
#include <map>

class Framework;
class Entity;
class Shader;

class Scene 
{
public:
	Scene(std::string a_sceneName, Framework* a_frameworkPointer);
	~Scene();

	virtual void Initialise() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;

	virtual void Draw() = 0;

	Framework* UseFramework() { return p_framework; }
	std::string GetSceneName() { return m_name; }

	Entity* GetEntity(std::string a_name);
	Shader* GetShader(std::string a_name);

	void AddEntity(std::string a_name, Entity* a_entity);
	void AddShader(std::string a_name, Shader* a_shader);

private:
	std::string m_name;
	Framework* p_framework;	

	std::map<std::string, Entity*> m_SceneEntities;
	std::map<std::string, Shader*> m_SceneShaders;
};