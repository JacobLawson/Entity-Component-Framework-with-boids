#include "InputManager.h"
#include <GLFW/glfw3.h>

InputManager::InputManager() 
{
	m_keyStates.resize(GLFW_KEY_LAST);
	m_mouseButtonStates.resize(GLFW_MOUSE_BUTTON_LAST);
	SetMouseOffsets(0.f, 0.f);
	SetMouseScrollOffsets(0.f, 0.f);
	m_firstMouse = true;
}

InputManager::~InputManager()
{
	m_keyStates.clear();
}


void InputManager::key_Callback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods)
{
	InputManager* im = InputManager::GetInstance();
	if (im != nullptr)
	{
		im->SetKeyState(a_key, a_action);
	}

}

void InputManager::mouse_Button_Callback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
	InputManager* im = InputManager::GetInstance();
	if (im != nullptr)
	{
		im->SetMouseButtonState(a_button, a_action);
	}
}

void InputManager::mouse_Move_Callback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager* im = InputManager::GetInstance();
	if (im != nullptr)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			if (im->GetFirstMouse())
			{
				im->SetMouseLastXY(xpos, ypos);
				im->SetFirstMouse(false);
			}

			float prevX; float prevY;
			im->GetMouseLastXY(prevX, prevY);
			im->AccumulateMouseOffsets(prevX - xpos, prevY - ypos);

			im->SetMouseLastXY(xpos, ypos);
		}
		else
		{
			im->SetFirstMouse(true);
		}
	}
}
void InputManager::mouse_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputManager* im = InputManager::GetInstance();
	if (im != nullptr)
	{
		im->AccumulateMouseScrollOffsets(xoffset, yoffset);
	}
}

uint8_t InputManager::GetKeyState(uint32_t a_keyVal)
{
	return m_keyStates[a_keyVal];
}

void InputManager::SetKeyState(int a_key, int a_state)
{
	if (a_key < m_keyStates.size())
	{
		m_keyStates[a_key] = a_state;
	}
}

void InputManager::SetMouseButtonState(int a_button, int a_state)
{
	if (a_button < m_mouseButtonStates.size())
	{
		m_mouseButtonStates[a_button] = a_state;
	}
}

uint8_t InputManager::GetMouseButtonState(uint32_t a_buttonVal)
{
	return m_mouseButtonStates[a_buttonVal];
}

void InputManager::SetMouseOffsets(float a_xOffset, float a_yOffset)
{
	m_mouseXOffset = a_xOffset;
	m_mouseYOffset = a_yOffset;
}

void InputManager::GetMouseOffsets(float& a_xOffset, float& a_yOffset)
{
	a_xOffset = m_mouseXOffset;
	a_yOffset = m_mouseYOffset;
}

void InputManager::AccumulateMouseOffsets(float a_xOffset, float a_yOffset)
{
	m_mouseXOffset += a_xOffset;
	m_mouseYOffset += a_yOffset;
}

void InputManager::SetMouseScrollOffsets(float a_xOffset, float a_yOffset)
{
	m_mouseScrollXOffset = a_xOffset;
	m_mouseScrollYOffset = a_yOffset;
}

void InputManager::GetMouseScrollOffsets(float& a_xOffset, float& a_yOffset)
{
	a_xOffset = m_mouseScrollXOffset;
	a_yOffset = m_mouseScrollYOffset;
}

void InputManager::AccumulateMouseScrollOffsets(float a_xOffset, float a_yOffset)
{
	m_mouseScrollXOffset += a_xOffset;
	m_mouseScrollYOffset += a_yOffset;
}
