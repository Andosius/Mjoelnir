#include "pch.h"

#include "CESP.hpp"

#include "Utils.hpp"


void CESP::Routine()
{
	while (m_Active)
	{

		// In case we are not alive we won't do anything
		if (!m_Client->GetLocalPlayer()->IsAlive())
			continue;

		m_Client->GetMutex()->lock();
		for (auto& target : *m_Client->GetPlayers())
		{
			glm::vec2 screen = {};
			
			bool tl = TraceLine(m_Client->GetBaseAddr(), m_Client->GetLocalPlayer()->GetOriginal(), target.GetHeadPosition(), true, true);
			bool w_to_s = OpenGL_WorldToScreen(m_Client->GetLocalPlayer()->GetHeadPosition(), m_Client->GetViewMatrix(), screen, m_Client->GetWindowData());
			if (tl && w_to_s)
			{
				// TODO: Draw stuff! :)
			}
		}
		m_Client->GetMutex()->unlock();
	}
}