#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>
#include <thread>

#define TARGET_KEY VK_INSERT

/*
* Inspired by https://guidedhacking.com/threads/universal-c-internal-dx9-imgui-overlay.19735/
* Pretty much doing the same, added a ton of comments and made my own version out of it.
*/

class Window
{
public:
	Window();
	~Window();

	void InitializeWindow(HMODULE hModule, const char* gameWindowClassName, const char* windowClassName, const char* windowName);

	HWND GetOverlayWindowHandle() { return m_WindowHandle; }
	D3DPRESENT_PARAMETERS* GetDirectXParams() { return &m_DirectXParams; }

	void RequestReset() { m_Change = true; }
	bool GetMenuVisibility() { return m_Visible; }
	void SetMenuVisibility(bool status) { m_Visible = status; }

	WNDPROC GetGameWindowProdecure() { return m_GameWindowProcedure; }

	void RenderDirectXFrame();

private:
	ATOM SetupWindow();
	void RenderImGui();

private:
	HMODULE m_HModule;
	const char* m_GameWindowClassName;
	const char* m_WindowClassName;
	const char* m_WindowName;

	HWND m_WindowHandle;
	WNDCLASSEXA m_WindowClass;

	DWORD m_GameProcessID;
	HWND m_GameWindowHandle;

	WNDPROC m_GameWindowProcedure;
	DWORD m_OriginalGameWindowProcedure;

	D3DPRESENT_PARAMETERS m_DirectXParams;
	LPDIRECT3DDEVICE9EX m_DirextXDevice;

	bool m_Change = false;
	bool m_Visible = false;

	bool m_Active = true;
	std::thread p_MessagesThread;

};


struct GameViewport {

	uint32_t X, Y, Width, Height;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowinfo
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-windowinfo
	// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
	static GameViewport GetGameWindowViewport(HWND gameWindowHandle)
	{
		WINDOWINFO windowInfo{};
		windowInfo.cbSize = sizeof(WINDOWINFO);

		GetWindowInfo(gameWindowHandle, &windowInfo);

		uint32_t windowWidth = windowInfo.rcWindow.right - windowInfo.rcWindow.left;
		uint32_t windowHeight = windowInfo.rcWindow.bottom - windowInfo.rcWindow.top;

		// Title bar height
		uint32_t titleBarHeight = 0;

		// We check for WS_BORDER because it would be included in WS_CAPTION and if it's set, there is going to be a border anyway.
		// So the only way is going for window client anyway
		// This is different to c(x/y)WindowBorders! Title bar height already includes cyWindowBorders!!!
		if (windowInfo.dwStyle & WS_BORDER)
		{
			titleBarHeight = windowHeight - (windowInfo.rcClient.bottom - windowInfo.rcClient.top);
		}

		GameViewport gVp{};
		gVp.X = windowInfo.rcWindow.left + windowInfo.cxWindowBorders;
		gVp.Y = windowInfo.rcWindow.top + (titleBarHeight - windowInfo.cyWindowBorders);
		gVp.Width = windowWidth;
		gVp.Height = windowHeight;

		return gVp;
	}
};

extern Window overlay;