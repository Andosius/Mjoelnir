#include "pch.h"

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

    DisableThreadLibraryCalls(hModule);

    Game client;
    client.Initialize();

    // Add Cheats here
    client.AddCheat(new CDebugger(&client, VK_NUMPAD5));
    client.AddCheat(new CAimbot(&client, VK_NUMPAD2));
    client.AddCheat(new CRecoil(&client, VK_NUMPAD3));
	client.AddCheat(new CAmmo(&client, VK_NUMPAD6));
	client.AddCheat(new CHealth(&client, VK_NUMPAD7));


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

	// Wait for threads to finish!
	Sleep(2000);

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
        CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

