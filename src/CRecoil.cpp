#include "pch.h"

#include "Constants.hpp"

#include "ICheat.hpp"
#include "CRecoil.hpp"


void CRecoil::Enable()
{
	Memory::Patch((BYTE*)(m_Client->GetBaseAddr() + RECOIL_FUNC), (BYTE*)"\xC2\x08\x00", 3);
}

void CRecoil::Disable()
{
	Memory::Patch((BYTE*)(m_Client->GetBaseAddr() + RECOIL_FUNC), (BYTE*)"\x55\x8B\xEC", 3);
}