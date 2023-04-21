#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>
#include <atlbase.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include "WindowViewport.hpp"

#ifndef DEBUG_PRINT
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#endif // !DEBUG_PRINT

#define TARGET_KEY VK_INSERT


struct ImGui_ImplDX9_Data;
struct ImGui_ImplWin32_Data;

extern ImGui_ImplDX9_Data* ImGui_ImplDX9_GetBackendData();
extern ImGui_ImplWin32_Data* ImGui_ImplWin32_GetBackendData();
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Overlay {

	// Main functions - don't use anything else! Easy to break!!!!!
	bool Setup();
	void Shutdown();
	void Routine();


	// DirectX 9 Functions
	bool CreateDirectX9Device(HWND handle, WindowViewport& view);
	void DestroyDirectX9Device();
	void ResetDirectX9Device();
	void RenderDirectX9Frame();

	// Overlay Functions
	void RenderOverlay();

	void InitializeParameter(HMODULE hInstance, const wchar_t* overlayedWindowClassName, LPSTR windowName, LPSTR windowClassName);
	ATOM CreateWindowClass();

	static LRESULT WINAPI WindowProcedure(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProcedureTimer(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
	static LRESULT CALLBACK OverlayedWindowProcedureHook(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam);

	// Helper functions
	HWND GetWindowHandle(DWORD processID, const wchar_t* overlayedWindowClassName);

}