#pragma once

static constexpr const char* WINDOW_CLASS_NAME = "Mjoelnir_Draw";
static constexpr const char* WINDOW_NAME = "Mjoelnir";

class Window
{
public:
	Window(HMODULE hModule, const char* gameWindowClassName, const char* windowClassName, const char* windowName);
	~Window() = default;

private:
	HMODULE m_HModule;
	const char* m_GameWindowClassName;
	const char* m_WindowClassName;
	const char* m_WindowName;

	HWND m_WindowHandle;

	DWORD m_GameProccessID;
	HWND m_GameWindowHandle;
};

