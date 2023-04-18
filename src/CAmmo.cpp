#include "pch.h"

#include "Constants.hpp"

#include "ICheat.hpp"
#include "CAmmo.hpp"

void CAmmo::Enable()
{
	Memory::Patch((BYTE*)(m_Client->GetBaseAddr() + DECREASE_AMMO_ADDRESS), (BYTE*)"\x90\x90", 2);
}

void CAmmo::Disable()
{
	Memory::Patch((BYTE*)(m_Client->GetBaseAddr() + DECREASE_AMMO_ADDRESS), (BYTE*)"\xFF\x0E", 2);
}