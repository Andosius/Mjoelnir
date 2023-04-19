#include "pch.h"

#include <iostream>
#include <memory>

#include "Game.hpp"
#include "CDebugger.hpp"
#include "CAimbot.hpp"
#include "CRecoil.hpp"
#include "CAmmo.hpp"
#include "CHealth.hpp"


DWORD WINAPI MainThread(HMODULE hModule)
{
	// Create console
	AllocConsole();
    FILE* f = nullptr;
	freopen_s(&f, "CONOUT$", "w", stdout);

    if (!f)
        return 0;

    Game client;
    client.Initialize();

    // Add Cheats here
    client.AddCheat(new CDebugger(&client, VK_NUMPAD5, "Debugger"));
    client.AddCheat(new CAimbot(&client, VK_NUMPAD2, "Aimbot"));
    client.AddCheat(new CRecoil(&client, VK_NUMPAD3, "NoRecoil"));
	client.AddCheat(new CAmmo(&client, VK_NUMPAD6, "AmmoHack"));
	client.AddCheat(new CHealth(&client, VK_NUMPAD7, "Invulnerability"));


    while (client.IsRunning())
	{
		client.Routine();
        Sleep(1);

        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            break;
        }
    }

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
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE thread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr);
        if (thread)
            CloseHandle(thread);
    }
}

