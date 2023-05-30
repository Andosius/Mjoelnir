#include <Windows.h>

#include <stdint.h>

struct WindowViewport {

	uint32_t X, Y, Width, Height;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowinfo
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-windowinfo
	// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
	static WindowViewport GetWindowViewport(HWND gameWindowHandle)
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

		WindowViewport gVp{};
		gVp.X = windowInfo.rcWindow.left + windowInfo.cxWindowBorders;
		gVp.Y = windowInfo.rcWindow.top + (titleBarHeight - windowInfo.cyWindowBorders);
		gVp.Width = windowWidth;
		gVp.Height = windowHeight;

		return gVp;
	}
};