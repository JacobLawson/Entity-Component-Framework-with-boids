#pragma once
#include "Manager.h"
#include <vector>

struct GLFWwindow;

class InputManager : public Manager<InputManager>
{
	friend class Manager<InputManager>;
protected:
	InputManager();
	virtual ~InputManager();

public:
	static void key_Callback(GLFWwindow* a_window, int a_key, int a_scanCode, int a_action, int a_mods);
	static void mouse_Button_Callback(GLFWwindow* a_window, int a_button, int a_action, int a_mods);
	static void mouse_Move_Callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
	uint8_t GetKeyState(uint32_t a_keyVal);
	void SetKeyState(int a_key, int a_state);

	void GetMouseLastXY(float& a_x, float& a_y) { a_x = m_lastMouseX;  a_y = m_lastMouseY; }
	void SetMouseLastXY(float a_x, float a_y) { m_lastMouseX = a_x;   m_lastMouseY = a_y; }
	void SetFirstMouse(bool a_val) { m_firstMouse = a_val; }
	bool GetFirstMouse() { return m_firstMouse; }
	//Set Mouse Button States
	void SetMouseButtonState(int a_button, int a_state);
	uint8_t GetMouseButtonState(uint32_t a_buttonVal);
	//Mouse Movement states
	void SetMouseOffsets(float a_xOffset, float a_yOffset);
	void GetMouseOffsets(float& a_xOffset, float& a_yOffset);
	void AccumulateMouseOffsets(float a_xOffset, float a_yOffset);
	//Mouse Scroll offsets
	void SetMouseScrollOffsets(float a_xOffset, float a_yOffset);
	void GetMouseScrollOffsets(float& a_xOffset, float& a_yOffset);
	float GetMouseScrollYOffset() { return m_mouseScrollYOffset; }
	void AccumulateMouseScrollOffsets(float a_xOffset, float a_yOffset);

private:
	std::vector<uint16_t> m_keyStates;
	std::vector<uint16_t> m_mouseButtonStates;

	float m_lastMouseX;
	float m_lastMouseY;
	bool m_firstMouse;
	float m_mouseXOffset;
	float m_mouseYOffset;
	float m_mouseScrollXOffset;
	float m_mouseScrollYOffset;
};