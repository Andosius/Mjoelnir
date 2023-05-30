#pragma once

#include "imgui.h"

#include "Overlay.hpp"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Windows.h>

extern void ImGui_ImplGlfw_CursorPosCallback(GLFWwindow* window, double x, double y);
extern void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
extern void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);


class Application {
public:
	Application(HMODULE hModule);
	~Application();
	void Run();

	static Application* Get();

	Game* GetClient() { return m_Client; }


	bool IsMenuOpened() { return m_Opened; }
	void ToggleMenu();

private:
	void Init();
	void SetupCheats();
	void SetupOverlay();
	void Cleanup();

private:
	HMODULE m_HModule = nullptr;

	Game* m_Client = nullptr;

	Overlay* m_Overlay = nullptr;
	FILE* m_Console = nullptr;

	bool m_Opened = false;
};
