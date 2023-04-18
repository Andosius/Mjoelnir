#pragma once

#include <Windows.h>
#include "Memory.hpp"

// ATTENTION: Hook32 TrampHooks only!
struct Hook32 {
	bool hkStatus = false;

	BYTE* src = nullptr;
	BYTE* dest = nullptr;
	BYTE* gateway = nullptr;
	uintptr_t len = 0;
	
	// We don't expect a hook to be greater than 10 bytes
	BYTE originalBytes[10]{ 0 };

	Hook32(BYTE* src, BYTE* dest, BYTE* gateway, uintptr_t len);
	Hook32(const char* exportName, const char* modName, BYTE* dest, BYTE* gateway, uintptr_t len);
	~Hook32();

	void Enable();
	void Disable();
	void Toggle();

};

bool Detour32(void* origin, void* substitute, int len);
BYTE* TrampHook32(BYTE* src, BYTE* BYTE, const intptr_t len);