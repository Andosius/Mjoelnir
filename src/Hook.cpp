#include "pch.h"

#include <Windows.h>

bool Detour32(void* origin, void* substitute, int len)
{
	if (len < 5)
		return false;

	DWORD curProtection;
	VirtualProtect(origin, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(origin, 0x90, len);

	DWORD relativeAddr = ((DWORD)substitute - (DWORD)origin) - 5;

	*(BYTE*)origin = 0xE9;
	*(DWORD*)((DWORD)origin + 1) = relativeAddr;

	DWORD tmp;
	VirtualProtect(origin, len, curProtection, &tmp);

	return true;
}

BYTE* TrampHook32(BYTE* src, BYTE* dst, const intptr_t len)
{
	// Make sure the length is greater than 5
	if (len < 5) 
		return nullptr;

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// In case something failed (and to prevent annoying VS messages, check if gateway is nullptr
	if (gateway == nullptr)
		return nullptr;

	//Write the stolen bytes into the gateway
	memcpy(gateway, src, len);

	// Get the gateway to destination addy
	intptr_t  gatewayRelativeAddr = (src - gateway) - 5;

	// Add the jmp opcode to the end of the gateway
	*(gateway + len) = 0xE9;

	// Add the address to the jmp
	*(intptr_t*)((intptr_t)gateway + len + 1) = gatewayRelativeAddr;

	// Perform the detour
	Detour32(src, dst, len);

	return gateway;
}

//https://guidedhacking.com/threads/ghb3-intermediate-guide-to-game-hacking.13495/
//https://guidedhacking.com/threads/simple-x86-c-trampoline-hook.14188/
//https://guidedhacking.com/threads/opengl-hooking-drawing-text-rendering-tutorial.14460/

//typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
//twglSwapBuffers owglSwapBuffers;
//
//#include <iostream>
//
//BOOL __stdcall hook_wglSwapBuffers(HDC hDc)
//{
//	std::cout << "Hooked" << std::endl;
//	return owglSwapBuffers(hDc);
//}
//
//DWORD WINAPI HackThread(HMODULE hModule)
//{
//	owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
//	owglSwapBuffers = (twglSwapBuffers)TrampHook32((BYTE*)owglSwapBuffers, (BYTE*)hook_wglSwapBuffers, 5);
//}

Hook32::Hook32(BYTE* src, BYTE* dest, BYTE* gateway, uintptr_t len)
{
	this->src = src;
	this->dest = dest;
	this->gateway = gateway;
	this->len = len;
}

Hook32::Hook32(const char* exportName, const char* modName, BYTE* dest, BYTE* gateway, uintptr_t len)
{
	HMODULE hModule = GetModuleHandleA(modName);
	this->src = (BYTE*)GetProcAddress(hModule, exportName);
	this->dest = dest;
	this->gateway = gateway;
	this->len = len;
}

Hook32::~Hook32()
{
	VirtualFree(gateway, 0, MEM_RELEASE);
}

void Hook32::Enable()
{
	memcpy(originalBytes, src, len);
	*(uintptr_t*)gateway = (uintptr_t)TrampHook32(src, dest, len);
	hkStatus = true;
}

void Hook32::Disable()
{
	Memory::Patch(src, originalBytes, len);
	hkStatus = false;
}

void Hook32::Toggle()
{

}
