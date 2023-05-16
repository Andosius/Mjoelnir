#include "pch.h"

#include <iostream>
#include <memory>

#include "Game.hpp"
#include "CDebugger.hpp"
#include "CAimbot.hpp"
#include "CRecoil.hpp"
#include "CAmmo.hpp"
#include "CHealth.hpp"
#include "Overlay.hpp"

#define OVERLAY_CLASS_NAME				L"SDL_app"
#define WINDOW_NAME						u8"Mjoelnir"
#define WINDOW_CLASS_NAME				u8"MjoelnirDraw"


DWORD WINAPI MainThread(HMODULE hModule)
{
	// Create console
    if (!AllocConsole())
        return 0;

    FILE* f = nullptr;

	freopen_s(&f, "CONOUT$", "w", stdout);
	//freopen_s(&f, "CONIN$", "r", stdin);
	//freopen_s(&f, "CONOUT$", "w", stderr);

    if (f == nullptr)
        return 0;

    Game client;
    client.Initialize();

    // Add Cheats here
    client.AddCheat(new CDebugger(&client, VK_NUMPAD5, "Debugger"));
    client.AddCheat(new CAimbot(&client, VK_NUMPAD2, "Aimbot"));
    client.AddCheat(new CRecoil(&client, VK_NUMPAD3, "NoRecoil"));
	client.AddCheat(new CAmmo(&client, VK_NUMPAD6, "AmmoHack"));
	client.AddCheat(new CHealth(&client, VK_NUMPAD7, "Invulnerability"));

	// Create overlay
	Overlay::InitializeParameter(hModule, OVERLAY_CLASS_NAME, WINDOW_NAME, WINDOW_CLASS_NAME);
	Overlay::Setup();

    while (client.IsRunning())
	{
		client.Routine();

		Overlay::Routine();

		if (GetAsyncKeyState(VK_NUMPAD9) & 1)
		{
			break;
		}

		Sleep(1);
    }

	Overlay::Shutdown();

    client.Cleanup();

    // Free console stuff
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);

    return 0;

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
		if (handle)
			CloseHandle(handle);
		break;
	}
	}
	return TRUE;
}

