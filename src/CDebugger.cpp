#include "pch.h"

#include <iostream>

#include "CDebugger.hpp"


void CDebugger::CheckInteraction()
{
	if (GetAsyncKeyState(m_Key) & 1)
	{
		//std::cout << "Current hp: " << m_LocalPlayer->playerent->m_Health << std::endl;
		//std::cout << "Current armor: " << m_LocalPlayer->playerent->m_Armor << std::endl;

		m_Client->GetLocalPlayer()->SetPlayerHealth(1337);
		m_Client->GetLocalPlayer()->SetPlayerArmor(1337);

		std::cout << "Current state: " << m_Client->GetLocalPlayer()->GetState() << std::endl;

		*m_Client->GetLocalPlayer()->GetWeapon()->w_CurrentAmmo = 555;
		

	}
}
