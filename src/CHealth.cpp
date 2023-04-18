#include "pch.h"

#include <iostream>

#include "Constants.hpp"
#include "Hook.hpp"

#include "CHealth.hpp"
#include "ICheat.hpp"


uintptr_t localPlayer = NULL;
uintptr_t returnFlow = NULL;


void __declspec(naked) ExtendedDamageCheck()
{
	// 6 = 6 bytes overwritten (see @Enable)
	__asm {
		cmp esi, [localPlayer]
		jne damage
		ret
		damage:
		push ebp
		mov ebp, esp
		and esp, -0x08
		jmp[returnFlow]
	}
}

void CHealth::Enable()
{
	localPlayer = (uintptr_t)(m_Client->GetLocalPlayer()->GetOriginal());
	//localPlayer = *(uintptr_t*)(m_Client->GetBaseAddr() + LOCAL_PLAYER_ADDRESS);
	returnFlow = m_Client->GetBaseAddr() + DAMAGE_CHECK_FUNC + 6;

	// We can't use a trampoline hook since we need to skip some bytes / instructions conditionally
	Detour32((void*)(m_Client->GetBaseAddr() + DAMAGE_CHECK_FUNC), ExtendedDamageCheck, 6);
}

void CHealth::Disable()
{
	Memory::Patch((BYTE*)(m_Client->GetBaseAddr() + DAMAGE_CHECK_FUNC), (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8", 6);
}