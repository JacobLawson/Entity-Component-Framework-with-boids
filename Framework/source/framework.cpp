#include <glad/glad.h>
#include <GLFW/glfw3.h>
//ImGui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "framework.h"
#include "InputManager.h"
#include <iostream>


Framework* Framework::m_Instance = nullptr;

Framework::Framework() : m_mainWindow(), m_transform(1.f) { m_Instance = this; }
Framework::~Framework() 
{ 
	m_Instance = nullptr; 
	delete m_camera;
}

bool Framework::Initialise(const char* a_windowName, const int a_width, const int a_height)
{
	m_windowWidth = a_width;
	m_windowHeight = a_height;

	// camera
   // m_lastX = a_width / 2.0f;
	//m_lastY = a_height / 2.0f;
	//m_firstMouse = true;

	// timing
	deltaTime = 0.0f;
	lastFrame = 0.0f;

	//initialise glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!glfwInit()) { return false; }	//If we cannot initialise glfw then exit failure

	GLFWwindow* window = glfwCreateWindow(a_width, a_height, a_windowName, nullptr, nullptr);
	if (!window) //Test Window has been successfully created
	{
		glfwTerminate();
		return false;
	}

	m_mainWindow = a_windowName;
	m_applicationWindows[a_windowName] = ((void*)window);

	//Create Input Manager instance
	m_InputManager = InputManager::CreateInstance();

	//Make window Context Current
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, InputManager::key_Callback);
	glfwSetMouseButtonCallback(window, InputManager::mouse_Button_Callback);
	glfwSetCursorPosCallback(window, InputManager::mouse_Move_Callback);
	glfwSetScrollCallback(window, InputManager::mouse_Scroll_Callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialise glad
	if (!gladLoadGL())
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	//GetVersion of Open GL Supported
	//OpenGLDebugVersion(0);


	//Initialise IMGUI
	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 150";
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Framework::GetCurrentProjectionView(glm::mat4& a_projection, glm::mat4& a_view)
{
	// view / projection transformations
	//a_projection = m_camera->GetProjectionMatrix();
	//a_view = m_camera->GetViewMatrix();
}

void Framework::Update()
{        
	// per-frame time logic
	// --------------------
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLFWwindow* window = (GLFWwindow*)(m_applicationWindows[m_mainWindow.c_str()]);


	//m_camera->Update(deltaTime);

	// input
	// -----
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	
}

void Framework::SwapBuffers()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	GLFWwindow* window = (GLFWwindow*)(m_applicationWindows[m_mainWindow.c_str()]);
	glfwSwapBuffers(window);

	InputManager* im = InputManager::GetInstance();
	im->SetMouseOffsets(0.f, 0.f);
	im->SetMouseScrollOffsets(0.f, 0.f);

	glfwPollEvents();
}

void Framework::ClearWindow()
{
	glClearColor(0.0f, 0.2f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framework::Destroy()
{
	for (auto iter = m_applicationWindows.begin(); iter != m_applicationWindows.end(); iter++)
	{
		GLFWwindow* window = (GLFWwindow*)(iter->second);
		glfwDestroyWindow(window);
	}
	m_InputManager->DestroyInstance();
	glfwTerminate();
}

bool Framework::MainWindowActive()
{
	GLFWwindow* window = (GLFWwindow*)(m_applicationWindows[m_mainWindow.c_str()]);
	return !glfwWindowShouldClose(window);
}

bool Framework::CreateWindow(std::string& a_windowName, const int a_width, const int a_height)
{
	GLFWwindow* window = glfwCreateWindow(a_width, a_height, a_windowName.c_str(), nullptr, nullptr);
	if (!window) //Test Window has been successfully created
	{
		return false;
	}
	m_applicationWindows[a_windowName] = ((void*)window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, InputManager::key_Callback);
	glfwSetMouseButtonCallback(window, InputManager::mouse_Button_Callback);
	glfwSetCursorPosCallback(window, InputManager::mouse_Move_Callback);
	glfwSetScrollCallback(window, InputManager::mouse_Scroll_Callback);
	return m_applicationWindows.size() - 1;
}

bool Framework::DestroyWindow(std::string& a_window)
{
	//Check to see if window has valid ID
	auto it = m_applicationWindows.find(a_window);
	if (it != m_applicationWindows.end())
	{
		GLFWwindow* window = (GLFWwindow*)(it->second);
		glfwDestroyWindow(window);
		m_applicationWindows.erase(it);
		return true;
	}
	return false;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Framework::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	Framework* fw = Framework::GetInstance();
	//CCamera* camera = fw->GetCamera();
	//camera->SetPerspective(60.f, (float)width / (float)height, 0.1f, 1000.f);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Framework::mouse_callback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{
	Framework* fw = Framework::GetInstance();
	InputManager* im = fw->GetInputManagerInstance();
	if (im->GetFirstMouse())
	{
		im->SetMouseLastXY(a_xpos, a_ypos);
		im->SetFirstMouse(false);
	}
	float prevX; float prevY;
	im->GetMouseLastXY(prevX, prevY);
	float xoffset = prevX - a_xpos;
	float yoffset = prevY - a_ypos;

	im->SetMouseLastXY(a_xpos, a_ypos);
}

void Framework::ImguiGetNewFrame()
{
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

//Debug
void Framework::OpenGLDebugVersion(std::string& a_window)
{
	//Check to see if window has valid ID
	auto it = m_applicationWindows.find(a_window);
	if (it != m_applicationWindows.end())
	{
		GLFWwindow* window = (GLFWwindow*)(it->second);
		int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
		int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
		int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);

		std::cout << "OpenGL Version Supported: " << major << "." << minor << "." << revision << std::endl;
	}
}

int Framework::RandomRange(int a_lowerRange, int a_higherRange)
{
	return rand() % (glm::abs(a_lowerRange - a_higherRange)) + a_lowerRange;
}
