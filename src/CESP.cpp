#include "pch.h"

#include "CESP.hpp"

#include "Utils.hpp"
#include "Application.hpp"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

constexpr float BODY_HEIGHT = 4.5f;
constexpr float SUB_HALF = 2.25f;
constexpr float RADIUS = 5.0f;


void CESP::Routine()
{
	// Clear draw data container
	m_DrawData.clear();

	// In case we are not alive we won't do anything
	if (!m_Client->GetLocalPlayer()->IsAlive())
		return;

	for (auto& target : *m_Client->GetPlayers())
	{
		glm::vec2 screen_pos{};
		glm::vec3 target_middle = target.GetHeadPosition() - glm::vec3(0.0f, 0.0f, SUB_HALF);
			
		// Get screen location
		glm::vec2 window = m_Client->GetWindowData();

		//bool on_screen = WorldToScreen(target_middle, screen_pos, matrix, (int)window.x, (int)window.y);
		bool on_screen = OpenGL_WorldToScreen(target_middle, m_Client->GetViewMatrix(), screen_pos, m_Client->GetWindowData());

		if (on_screen && m_Active)
		{
			// Enemies red, teammates green
			/*
			x = R
			y = G
			z = B
			w = A
			*/
			ImColor color = ImColor(0, 0, 0, 255);
			if (target.GetTeam() != m_Client->GetLocalPlayer()->GetTeam())
				color.Value.x = 255;
			else
				color.Value.y = 255;

			// Add circle
			DrawData data = { ImVec2(screen_pos.x, screen_pos.y), ImVec2(), color, RADIUS, DrawType::Circle };

			m_DrawData.push_back(data);
		}
	}
}