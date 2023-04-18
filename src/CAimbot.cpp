#include "pch.h"

#include <iostream>
#include <algorithm>

#include "Constants.hpp"

#include "CAimbot.hpp"
#include "Utils.hpp"


void CAimbot::Routine()
{
	while (m_Active)
	{
		//std::cout << "Aimbot[" << this->GetThread()->get_id() << "] routine!" << std::endl;

		// In case we are not alive we won't do anything
		if (!m_Client->GetLocalPlayer()->IsAlive())
			continue;

		m_Client->GetMutex()->lock();
		for (auto& target : *m_Client->GetPlayers())
		{
			while (m_Active && target.IsAlive() && TraceLine(m_Client->GetBaseAddr(), m_Client->GetLocalPlayer()->GetOriginal(), target.GetHeadPosition()))
			{
				m_HasTarget = true;
				m_Client->GetLocalPlayer()->SetPlayerHeadRotation(CalculateHeadPosition(m_Client->GetLocalPlayer()->GetHeadPosition(), target.GetHeadPosition()));
				m_Client->GetLocalPlayer()->SetPlayerAttacking(true);
			}
			if (m_HasTarget)
			{
				m_HasTarget = false;
				m_Client->GetLocalPlayer()->SetPlayerAttacking(false);
			}
		}
		m_Client->GetMutex()->unlock();
	}
}
