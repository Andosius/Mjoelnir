#include "pch.h"
#include "C:/Users/Denis/Desktop/Mjoelnir-cmake/build/CMakeFiles/Mjoelnir.dir/Release/cmake_pch.hxx"

#include "Application.hpp"

#include "Game.hpp"
#include "CDebugger.hpp"
#include "CAimbot.hpp"
#include "CRecoil.hpp"
#include "CAmmo.hpp"
#include "CHealth.hpp"

#include "IDrawing.hpp"

static Application* s_Instance = nullptr;


Application::Application(HMODULE hModule)
	: m_HModule(hModule)
{
	s_Instance = this;

	Init();
	SetupCheats();
	SetupOverlay();
}

Application::~Application()
{
	Cleanup();
}

void Application::Init()
{
	// In case we can't allocate a console, we won't proceed
	if (!AllocConsole())
		return;
	
	// Open stdout session
	freopen_s(&m_Console, "CONOUT$", "w", stdout);

	// In case it failed we won't proceed
	if (m_Console == nullptr)
		return;
}

void Application::SetupCheats()
{
	m_Client = new Game();

	m_Client->Initialize();
	m_Client->AddCheat(new CDebugger(m_Client, VK_NUMPAD5, "Debugger"));
	m_Client->AddCheat(new CAimbot(m_Client, VK_NUMPAD2, "Aimbot"));
	m_Client->AddCheat(new CRecoil(m_Client, VK_NUMPAD3, "NoRecoil"));
	m_Client->AddCheat(new CAmmo(m_Client, VK_NUMPAD6, "AmmoHack"));
	m_Client->AddCheat(new CHealth(m_Client, VK_NUMPAD7, "Invulnerability"));

}

void Application::SetupOverlay()
{
	OverlaySpecification specs = {};
	specs.Name = "Mjoelnir";

	m_Overlay = new Overlay(specs);
}

void Application::Cleanup()
{
	m_Client->Cleanup();
	m_Overlay->Close();

	delete m_Overlay;
	delete m_Client;

	fclose(m_Console);
	FreeConsole();
}

void Application::Run()
{
	while (m_Client->IsRunning())
	{
		// Call subroutines
		m_Client->Routine();
		m_Overlay->Routine();

		if (GetAsyncKeyState(VK_NUMPAD9) & 1)
		{
			break;
		}

		Sleep(1);
	}
}

void Overlay::OnUpdate(float ts)
{
	if (s_Instance == nullptr)
		return;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	if (ImGui::Begin("Draw", nullptr, flags))
	{
		ImGui::Text("Render time: %0.4f", ts);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(ImVec2(0.0f, 0.0f), ImVec2(500.0f, 1000.0f), ImColor(255, 255, 255, 255), 1.0f);
// 		for (auto& cheat : *s_Instance->GetClient()->GetCheats())
// 		{
// 			// Check if Cheat is a Drawing Cheat
// 			if (IDrawing* drawing = dynamic_cast<IDrawing*>(cheat))
// 			{
// 
// 			}
// 		}

		ImGui::End();
	}

	ImGui::PopStyleVar(3);
}