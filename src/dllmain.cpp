#include "pch.h"

#include <iostream>
#include <memory>

#include "Application.hpp"

DWORD WINAPI MainThread(HMODULE hModule)
{
	Application* app = new Application(hModule);
	app->Run();

	delete app;
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

