#pragma once
//Windows Includes
#include <map>
#include <string>
//GLM includes
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//Model Loading
#include <learnopengl/include/shader.h>
#include <learnopengl/include/model.h>

#include "Camera.h"

class InputManager;

//forward declare struct for glfwwindow
struct GLFWwindow;

class Framework
{
public:
	Framework();
	~Framework();

	bool Initialise(const char* a_windowName, const int a_width, const int a_height);
	void Update();
	void Destroy();
	void ClearWindow();
	void SwapBuffers();

	bool MainWindowActive();
	bool CreateWindow(std::string& a_windowName, const int a_width, const int a_height);
	bool DestroyWindow(std::string& a_window);

	//GLFW window callback functions
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* a_window, double a_xpos, double a_ypos);


	static Framework* GetInstance() { return m_Instance; }
	CCamera* GetCamera() { return m_camera; }
	
	void GetCurrentProjectionView(glm::mat4& a_projection, glm::mat4& a_view);

	float GetDeltaTime() { return deltaTime; }

	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }
	
	InputManager* GetInputManagerInstance() { return m_InputManager; }

	//Randomization
	int RandomRange(int a_lowerRange, int a_higherRange);

	//Debug
	void OpenGLDebugVersion(std::string& a_window);

	void ImguiGetNewFrame();

private:
	static Framework* m_Instance;
	InputManager* m_InputManager;

	int m_windowWidth;
	int m_windowHeight;

	CCamera* m_camera;
	glm::mat4 m_transform;

	Shader* m_ourShader;
	Model* m_ourModel;

	//timing
	float deltaTime;
	float lastFrame;

	std::string m_mainWindow;
	std::map<std::string, void*> m_applicationWindows;
};