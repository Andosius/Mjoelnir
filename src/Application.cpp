#include "pch.h"

#include "Application.hpp"

#include "Game.hpp"
#include "CDebugger.hpp"
#include "CAimbot.hpp"
#include "CRecoil.hpp"
#include "CAmmo.hpp"
#include "CHealth.hpp"
#include "CESP.hpp"

#include "IDrawing.hpp"

#include <GLFW/glfw3.h>

static Application* s_Instance = nullptr;

#define _DEBUG

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
#ifdef _DEBUG
	// In case we can't allocate a console, we won't proceed
	if (!AllocConsole())
		return;
	
	// Open stdout session
	freopen_s(&m_Console, "CONOUT$", "w", stdout);

	// In case it failed we won't proceed
	if (m_Console == nullptr)
		return;
#endif
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
	m_Client->AddCheat(new CESP(m_Client, VK_NUMPAD1, "ESP"));
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

	delete m_Client;
	delete m_Overlay;

#ifdef _DEBUG
	fclose(m_Console);
	FreeConsole();
#endif
}

void Application::Run()
{

	while (m_Client->IsRunning())
	{
		// Call subroutines
		m_Client->Routine();
		m_Overlay->Routine();

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			ToggleMenu();
		}

		if (GetAsyncKeyState(VK_NUMPAD9) & 1)
		{
			break;
		}

		Sleep(1);
	}
}

Application* Application::Get()
{
	return s_Instance;
}

void Application::ToggleMenu()
{
	auto window = m_Overlay->GetWindowHandle();

	m_Opened = !m_Opened;
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, !m_Opened);

	ImGuiIO& io = ImGui::GetIO();

	if (m_Opened)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show cursor
		io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;          // Enable Mouse
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard


		glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
		glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
		glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor
		io.ConfigFlags |= ImGuiConfigFlags_NoMouse;            // Disable Mouse
		io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Disable Keyboard

		glfwSetCursorPosCallback(window, nullptr);
		glfwSetMouseButtonCallback(window, nullptr);
		glfwSetKeyCallback(window, nullptr);
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

	ImGuiWindowFlags flags = 0;
	if (s_Instance->IsMenuOpened())
	{
		flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	}
	else
	{
		flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs;
	}
	if (ImGui::Begin("Draw", nullptr, flags))
	{
		/*
			TODO: Draw Menu!
		*/

		ImGuiIO& io = ImGui::GetIO();

		if (s_Instance->IsMenuOpened())
			ImGui::Text("Render time: %0.4f", ts);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Go through all IDrawing cheats
		for (auto& cheat : *s_Instance->GetClient()->GetCheats())
		{
			// It can be cast to IDrawing pointer? The class must inherit from IDrawing!
			if (IDrawing* drawing = dynamic_cast<IDrawing*>(cheat))
			{
				if (!cheat->IsActive())
					continue;

				cheat->Routine();

				for (const DrawData& draw : *drawing->GetDrawData())
				{
					if (draw.Type == DrawType::Line)
					{
						drawList->AddLine(draw.Start, draw.End, draw.Color);
					}
					else if (draw.Type == DrawType::Circle)
					{
						drawList->AddCircle(draw.Start, draw.Radius, draw.Color);
					}
				}
			}
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(3);
}