#include "pch.h"
#include "Window.hpp"

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <iostream>

Window overlay;


// inspired by:
// https://www.codeproject.com/Questions/1224870/How-to-find-multiple-window-handles-from-a-process
HWND GetWindowHandle(DWORD processID, const wchar_t* gameWindowClassName)
{
	HWND currentWindow = NULL;
	do 
	{
		currentWindow = FindWindowEx(NULL, currentWindow, gameWindowClassName, NULL);
		DWORD cwProcID = 0;
		GetWindowThreadProcessId(currentWindow, &cwProcID);

		if (cwProcID == processID)
			return currentWindow;
	} 
	while (currentWindow != NULL);

	return NULL;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK GameWindowProcedureHook(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if (message == WM_WINDOWPOSCHANGED)
	{
		// In case the game window moves we have to get new information from lParam and GetWindowInfo for additional info like
		// borders, etc. so we can stay on top of the window
		WINDOWPOS* windowPositionStructure = reinterpret_cast<WINDOWPOS*>(lParam);
		WINDOWINFO windowInfo{};
		windowInfo.cbSize = sizeof(WINDOWINFO);

		GetWindowInfo(windowPositionStructure->hwnd, &windowInfo);
		
		if (windowInfo.dwStyle & WS_BORDER)
		{
			int32_t windowTitleHeight = (windowInfo.rcWindow.bottom - windowInfo.rcWindow.top) - (windowInfo.rcClient.bottom - windowInfo.rcClient.top);
			windowPositionStructure->y += windowTitleHeight;
			windowPositionStructure->cy -= windowTitleHeight;
		}

		auto params = overlay.GetDirectXParams();

		params->BackBufferWidth = windowPositionStructure->cx;
		params->BackBufferHeight = windowPositionStructure->cy;

		ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(windowPositionStructure->cx), static_cast<float>(windowPositionStructure->cy));

		// Move overlay window to new position
		MoveWindow(overlay.GetOverlayWindowHandle(), windowPositionStructure->x + windowInfo.cxWindowBorders, windowPositionStructure->y - windowInfo.cyWindowBorders, windowPositionStructure->cx, windowPositionStructure->cy, false);

		overlay.RequestReset();
	}

	if (message == WM_KEYUP && wParam == TARGET_KEY)
	{
		overlay.SetMenuVisibility(!overlay.GetMenuVisibility());
	}

	if (overlay.GetMenuVisibility())
	{
		bool menuClicked = false;

		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureMouse && (message == WM_LBUTTONDOWN && wParam == VK_LBUTTON))
		{
			menuClicked = true;
		}

		// In case the menu is open and we did not click on ImGui AND we are holding down the button -> redirect to game
		if (menuClicked)
		{
			if (message == WM_KEYDOWN && wParam == TARGET_KEY)
			{
				return CallWindowProcA(reinterpret_cast<WNDPROC>(overlay.GetGameWindowProdecure()), handle, WM_KEYUP, wParam, lParam);
			}

			ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
			return true;
		}
	}
	return CallWindowProcA(reinterpret_cast<WNDPROC>(overlay.GetGameWindowProdecure()), handle, message, wParam, lParam);
}

Window::Window()
	: m_WindowClass{}, m_DirectXParams{}
{
	m_HModule = NULL;
	m_GameWindowClassName = nullptr;
	m_WindowClassName = nullptr;
	m_WindowName = nullptr;
	
	m_WindowHandle = NULL;
	
	// Game process ID is current processID since we are internal.
	m_GameProcessID = GetCurrentProcessId();
	m_GameWindowHandle = NULL;
}

Window::~Window()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	DestroyWindow(m_WindowHandle);
	if (m_DirextXDevice) m_DirextXDevice->Release();
	UnregisterClassA(m_WindowClassName, m_HModule);

	SetWindowLongPtrA(m_GameWindowHandle, GWLP_WNDPROC, LONG_PTR(m_OriginalGameWindowProcedure));
}

void Window::InitializeWindow(HMODULE hModule, const char* gameWindowClassName, const char* windowClassName, const char* windowName)
{
	m_GameWindowHandle = GetWindowHandle(m_GameProcessID, (wchar_t*)windowClassName);

	m_HModule = hModule;
	m_GameWindowClassName = gameWindowClassName;
	m_WindowClassName = windowClassName;
	m_WindowName = windowName;

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.DeltaTime = 1.0f / 60.0f;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	// Setup window class and cross fingers that creation works, otherwise we stop execution
	if (!SetupWindow())
	{
		return;
	}

	// Get accurate window positions for window position and size
	GameViewport game = GameViewport::GetGameWindowViewport(m_GameWindowHandle);

	// https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
	// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-features#layered-windows
	/*
		WS_EX_TOPMOST = top layer
		WS_EX_LAYERED = enables the possibility to make the window blend, all windows below will get updates tho
		WS_EX_TRANSPARENT = enables the possibility to make the window alpha 0
		WS_EX_TOOLWINDOW = makes is invisible in Task Manager and you can't ALT + TAB it
	*/
	DWORD style = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW;

	// Window creation as GameWindowHandle child
	// WS_POPUP is the raw form of an WS_POPUPWINDOW which does not contain a WS_BORDER or WS_SYSMENU
	m_WindowHandle = CreateWindowExA(style, m_WindowClassName, m_WindowName, WS_POPUP, game.X, game.Y, game.Width, game.Height, m_GameWindowHandle, NULL, m_HModule, NULL);

	// Window magic to make it transparent:
	// BOOL SetLayeredWindowAttributes([in] HWND hwnd,[in] COLORREF crKey, [in] BYTE bAlpha, [in] DWORD dwFlags);
	// LWA_ALPHA: Use bAlpha to determine the opacity of the layered window. => The stuff we draw on top
	// LWA_COLORKEY: Use crKey as the transparency color.
	SetLayeredWindowAttributes(m_WindowHandle, NULL, 255, LWA_ALPHA);
	SetLayeredWindowAttributes(m_WindowHandle, NULL, RGB(0, 0, 0), LWA_COLORKEY);

	// Set ImGui Display size
	io.DisplaySize = ImVec2(static_cast<float>(game.Width), static_cast<float>(game.Height));


	// Check for DirectX 9 before doing something
	LPDIRECT3D9EX instance;
	HRESULT check = Direct3DCreate9Ex(D3D_SDK_VERSION, &instance);

	if (FAILED(check))
		return;

	// Setup Device
	ZeroMemory(&this->m_DirectXParams, sizeof(this->m_DirectXParams));


	m_DirectXParams.Windowed = TRUE;
	m_DirectXParams.hDeviceWindow = m_WindowHandle;
	m_DirectXParams.BackBufferWidth = game.Width;
	m_DirectXParams.BackBufferHeight = game.Height;
	m_DirectXParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_DirectXParams.BackBufferFormat = D3DFMT_UNKNOWN;
	m_DirectXParams.EnableAutoDepthStencil = TRUE;
	m_DirectXParams.AutoDepthStencilFormat = D3DFMT_D16;
	m_DirectXParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (instance->CreateDeviceEx(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_WindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_DirectXParams, 0, &m_DirextXDevice) != D3D_OK)
	{
		instance->Release();
		return;
	}

	// Initialize Dear ImGui
	ImGui_ImplWin32_Init(m_GameWindowHandle);
	ImGui_ImplDX9_Init(m_DirextXDevice);

	ImGui::StyleColorsDark();

	// Update & show window
	UpdateWindow(m_WindowHandle);
	ShowWindow(m_WindowHandle, SW_SHOW);

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra
	// Hook GameWindowProcedure so we have control over what is pressed and have control over all types of events
	m_OriginalGameWindowProcedure = GetWindowLongPtrA(m_GameWindowHandle, GWL_WNDPROC);
	m_GameWindowProcedure = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(m_GameWindowHandle, GWLP_WNDPROC, LONG_PTR(GameWindowProcedureHook)));
}


// https://learn.microsoft.com/en-us/windows/win32/winmsg/using-timers
static LRESULT CALLBACK WindowProcedureTimer(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	overlay.RenderDirectXFrame();
	/*
	
		TODO:
		- Render-Function calls

	*/
	return TRUE;
}

// If STRICT is defined, the lpPrevWndFunc parameter has the data type WNDPROC. The WNDPROC type is declared as follows:
// LRESULT (CALLBACK* WNDPROC) (HWND, UINT, WPARAM, LPARAM); 
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca
// https://learn.microsoft.com/en-us/windows/win32/winmsg/using-window-procedures (watch this in case you want to understand what's happening)
static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// [in] uElapse: The time-out value, in milliseconds.
	//constexpr UINT TIMER_DELAY = 50;
	static constexpr uint32_t DELAY = (1000 / 30);
	switch (message)
	{
	case WM_CREATE:
		SetTimer(handle, NULL, DELAY, reinterpret_cast<TIMERPROC>(&WindowProcedureTimer));
		break;
	case WM_DESTROY:
		KillTimer(handle, NULL);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
		break;
	}
	return 0;
}

ATOM Window::SetupWindow()
{
	// All members are preset to be 0 thanks to the constructor, so no big deal setting the necessaery values.

	// The size, in bytes, of this structure. Set this member to sizeof(WNDCLASSEX).
	m_WindowClass.cbSize = sizeof(WNDCLASSEX);

	// A handle to the class background brush. This member can be a handle to the brush to be used for painting the background, or it can be a color value.
	m_WindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	// We set it to our instance so it's a part of the game and can be overlaying in full screen mode
	m_WindowClass.hInstance = m_HModule;

	// A pointer to the window procedure. You must use the CallWindowProc function to call the window procedure. For more information, see WindowProc.
	m_WindowClass.lpfnWndProc = WindowProcedure;

	// If lpszClassName is a string, it specifies the window class name. The class name can be any name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names.
	m_WindowClass.lpszClassName = m_WindowClassName;

	// Pointer to a null-terminated character string that specifies the resource name of the class menu, as the name appears in the resource file.
	m_WindowClass.lpszMenuName = m_WindowName;

	m_WindowClass.style = CS_VREDRAW | CS_HREDRAW;

	return RegisterClassExA(&m_WindowClass);
}

void Window::RenderDirectXFrame()
{
	if (GetForegroundWindow() == m_GameWindowHandle && m_Visible)
	{
		// Recreate device settings in case something changed!
		if (m_Change)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			m_DirextXDevice->Reset(&m_DirectXParams);
			ImGui_ImplDX9_CreateDeviceObjects();

			m_Change = false;
		}
		
		m_DirextXDevice->BeginScene();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		RenderImGui();

		ImGui::EndFrame();

		m_DirextXDevice->SetRenderState(D3DRS_ZENABLE, false);
		m_DirextXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_DirextXDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		m_DirextXDevice->EndScene();
	}

	m_DirextXDevice->Present(NULL, NULL, NULL, NULL);
	m_DirextXDevice->Clear(NULL, NULL, D3DCLEAR_TARGET, NULL, 1.0f, NULL);
}

void Window::RenderImGui()
{
	if (ImGui::Begin("GH Internal ImGui Overlay", &this->m_Visible))
	{
		ImGui::Text("Some bullshit text");

		ImGui::End();
	}
}