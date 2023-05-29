#pragma once

#include "imgui.h"

#include "Overlay.hpp"

#include <Windows.h>

class Application {
public:
	Application(HMODULE hModule);
	~Application();
	void Run();

	Game* GetClient() { return m_Client; }

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
};
