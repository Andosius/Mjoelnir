#include "pch.h"

#include "Overlay.hpp"

#include "IDrawing.hpp"
#include "Utils.hpp"

#include <iostream>
#include <map>
#include <mutex>

// DirectX 9 Data
static LPDIRECT3D9                      g_Instance = nullptr;
static LPDIRECT3DDEVICE9                g_Device = nullptr;
static D3DPRESENT_PARAMETERS            g_PresentParams = {};

// Overlay Data
static HMODULE                          s_Instance = NULL;
static LPSTR                            s_WindowName = nullptr;
static LPSTR                            s_WindowClassName = nullptr;

static DWORD                            s_OverlayedProcessID = NULL;
static HWND                             s_OverlayedHandle = NULL;
static WNDPROC                          s_OverlayedOriginalProcedure = nullptr;

static HWND                             s_WindowHandle = NULL;
static WNDCLASSA                        s_WindowClass = {};

static bool                             s_WindowRegistered = false;

static bool                             s_Visible = false;
static bool                             s_ResetRequested = false;

static std::map<std::string, IDrawing*> s_Plugins = {};
static std::vector<Vertices>            s_Vertices(500);
static std::mutex                       s_Mutex = {};

namespace Overlay {

    void InitializeParameter(HMODULE hInstance, const wchar_t* overlayedWindowClassName, LPSTR windowName, LPSTR windowClassName)
    {
        s_Instance = hInstance;
        s_WindowName = windowName;
        s_WindowClassName = windowClassName;

        s_OverlayedProcessID = GetCurrentProcessId();
        s_OverlayedHandle = GetWindowHandle(s_OverlayedProcessID, overlayedWindowClassName);
    }

    bool Setup()
    {
        // Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.DeltaTime = 1.0f / 60.0f;
        io.IniFilename = nullptr;
        io.Fonts->AddFontDefault();

        // Setup window class and cross fingers that creation works, otherwise we stop execution
        if (!CreateWindowClass())
        {
            return false;
        }

        s_WindowRegistered = true;

        // Get accurate window positions for window position and size
        WindowViewport viewport = WindowViewport::GetWindowViewport(s_OverlayedHandle);

        /*
            https://learn.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
            https://learn.microsoft.com/en-us/windows/win32/winmsg/window-features#layered-windows
            WS_EX_TOPMOST = top layer
            WS_EX_LAYERED = enables the possibility to make the window blend, all windows below will get updates tho
            WS_EX_TRANSPARENT = enables the possibility to make the window alpha 0
            WS_EX_TOOLWINDOW = makes is invisible in Task Manager and you can't ALT + TAB it
        */
        DWORD style = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW;

        // Window creation as GameWindowHandle child
        // WS_POPUP is the raw form of an WS_POPUPWINDOW which does not contain a WS_BORDER or WS_SYSMENU
        s_WindowHandle = CreateWindowExA(style, s_WindowClassName, s_WindowName, WS_POPUP, viewport.X, viewport.Y, viewport.Width, viewport.Height, s_OverlayedHandle, NULL, s_Instance, NULL);

        SetLayeredWindowAttributes(s_WindowHandle, NULL, 255, LWA_ALPHA);
        SetLayeredWindowAttributes(s_WindowHandle, NULL, RGB(0, 0, 0), LWA_COLORKEY);

        io.DisplaySize = ImVec2(static_cast<float>(viewport.Width), static_cast<float>(viewport.Height));

        if (!CreateDirectX9Device(s_WindowHandle, viewport))
        {
            Shutdown();
        }

        // Initialize Dear ImGui
        ImGui_ImplWin32_Init(s_OverlayedHandle);
        ImGui_ImplDX9_Init(g_Device);

        ImGui::StyleColorsDark();

        // Update & show window
        UpdateWindow(s_WindowHandle);
        ShowWindow(s_WindowHandle, SW_SHOW);

        // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra
        // Hook GameWindowProcedure so we have control over what is pressed and have control over all types of events
        s_OverlayedOriginalProcedure = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(s_OverlayedHandle, GWLP_WNDPROC, LONG_PTR(OverlayedWindowProcedureHook)));

        return true;
    }

    void Shutdown()
    {
        if (ImGui_ImplDX9_GetBackendData != nullptr)
            ImGui_ImplDX9_Shutdown();
        if (ImGui_ImplWin32_GetBackendData != nullptr)
            ImGui_ImplWin32_Shutdown();
        if (ImGui::GetCurrentContext() != nullptr)
            ImGui::DestroyContext();

        DestroyDirectX9Device();

        if (s_OverlayedOriginalProcedure != nullptr)
            SetWindowLongPtrA(s_OverlayedHandle, GWLP_WNDPROC, LONG_PTR(s_OverlayedOriginalProcedure));

        if (s_WindowHandle)
            ::DestroyWindow(s_WindowHandle);
        if (s_WindowRegistered)
            ::UnregisterClassA(s_WindowClass.lpszClassName, s_WindowClass.hInstance);
    }

	void Routine()
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	bool CreateDirectX9Device(HWND handle, WindowViewport& view)
    {
        if ((g_Instance = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
            return false;

        // Create DirectX 9 Device
        ZeroMemory(&g_PresentParams, sizeof(g_PresentParams));

        g_PresentParams.Windowed = TRUE;
        g_PresentParams.hDeviceWindow = s_WindowHandle;
        g_PresentParams.BackBufferWidth = view.Width;
        g_PresentParams.BackBufferHeight = view.Height;
        g_PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_PresentParams.BackBufferFormat = D3DFMT_UNKNOWN;
        g_PresentParams.EnableAutoDepthStencil = TRUE;
        g_PresentParams.AutoDepthStencilFormat = D3DFMT_D16;
        g_PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

        if (g_Instance->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            handle,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &g_PresentParams,
            &g_Device
        ) != D3D_OK)
        {
            g_Instance->Release();
            return false;
        }

        return true;
    }

    void DestroyDirectX9Device()
    {
        if (g_Instance)
        {
            g_Instance->Release();
            g_Instance = nullptr;
        }

        if (g_Device)
        {
            g_Device->Release();
            g_Device = nullptr;
        }
    }

    void ResetDirectX9Device()
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        g_Device->Reset(&g_PresentParams);
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    void RenderDirectX9Frame()
    {

        if (GetForegroundWindow() == s_OverlayedHandle && s_Visible)
        {
            // Window moved? Change position!
            if (s_ResetRequested)
            {
                ImGui_ImplDX9_InvalidateDeviceObjects();
                g_Device->Reset(&g_PresentParams);
                ImGui_ImplDX9_CreateDeviceObjects();

                s_ResetRequested = false;
            }

            g_Device->BeginScene();

            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();

            RenderOverlay();

            ImGui::EndFrame();

            g_Device->SetRenderState(D3DRS_ZENABLE, false);
            g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
            g_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

            g_Device->EndScene();

        }

		g_Device->Present(NULL, NULL, NULL, NULL);
		g_Device->Clear(NULL, NULL, D3DCLEAR_TARGET, NULL, 1.0f, NULL);

    }

    void RenderOverlay()
    {
        if (ImGui::Begin("Internal Overlay", &s_Visible))
        {
            ImGui::Text("Oh lord, let's go!");
            ImGui::End();
        }
    }

    ATOM CreateWindowClass()
    {
        USES_CONVERSION;

        // The size, in bytes, of this structure. Set this member to sizeof(WNDCLASSEX).
        //s_WindowClass.cbSize = sizeof(WNDCLASSEX);

        // A handle to the class background brush. This member can be a handle to the brush to be used for painting the background, or it can be a color value.
        s_WindowClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

        // We set it to our instance so it's a part of the game and can be overlaying in full screen mode
        s_WindowClass.hInstance = s_Instance;

        // A pointer to the window procedure. You must use the CallWindowProc function to call the window procedure. For more information, see WindowProc.
        s_WindowClass.lpfnWndProc = WindowProcedure;

        // If lpszClassName is a string, it specifies the window class name. The class name can be any name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names.
        s_WindowClass.lpszClassName = s_WindowClassName;

        // Pointer to a null-terminated character string that specifies the resource name of the class menu, as the name appears in the resource file.
        s_WindowClass.lpszMenuName = s_WindowName;

        s_WindowClass.style = CS_VREDRAW | CS_HREDRAW;

        return RegisterClassA(&s_WindowClass);
    }

    static LRESULT WINAPI WindowProcedure(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // [in] uElapse: The time-out value, in milliseconds.
        constexpr UINT TIMER_DELAY = 50;
        switch (msg)
        {
        case WM_CREATE:
            SetTimer(handle, NULL, TIMER_DELAY, reinterpret_cast<TIMERPROC>(&WindowProcedureTimer));
            break;
        case WM_DESTROY:
            KillTimer(handle, NULL);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(handle, msg, wParam, lParam);
            break;
        }
        return 0;
    }

    static LRESULT CALLBACK WindowProcedureTimer(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
    {
        RenderDirectX9Frame();

        if (s_Vertices.size() > 0)
        {
            // check _D3DPRIMITIVETYPE out!
        }

        return TRUE;
    }

    HWND GetWindowHandle(DWORD processID, const wchar_t* overlayedWindowClassName)
    {
        // modified version of https://www.codeproject.com/Questions/1224870/How-to-find-multiple-window-handles-from-a-process
        HWND currentWindow = NULL;
        do
        {
            currentWindow = FindWindowEx(NULL, currentWindow, overlayedWindowClassName, NULL);
            DWORD cwProcID = 0;
            GetWindowThreadProcessId(currentWindow, &cwProcID);

            if (cwProcID == processID)
                return currentWindow;
        } while (currentWindow != NULL);

        return NULL;
    }

    static LRESULT CALLBACK OverlayedWindowProcedureHook(const HWND handle, const UINT message, const WPARAM wParam, const LPARAM lParam)
    {
        // TODO: rewrite to own code (WindowViewport!!!!)
        if (message == WM_WINDOWPOSCHANGED)
        {
            // In case the overlayed window moves we have to get new information from lParam and GetWindowInfo for additional info like
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

            g_PresentParams.BackBufferWidth = windowPositionStructure->cx;
            g_PresentParams.BackBufferHeight = windowPositionStructure->cy;

            ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(windowPositionStructure->cx), static_cast<float>(windowPositionStructure->cy));

            // Move overlay window to new position
            MoveWindow(s_WindowHandle, windowPositionStructure->x + windowInfo.cxWindowBorders, windowPositionStructure->y - windowInfo.cyWindowBorders, windowPositionStructure->cx, windowPositionStructure->cy, false);

            s_ResetRequested = true;
        }

        if (message == WM_KEYUP && wParam == TARGET_KEY)
        {
            s_Visible = !s_Visible;
        }

        // In case the menu is open, we don't wanna send anything to the game
        if (s_Visible)
        {
            ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
            return true;
        }
        return CallWindowProcA(reinterpret_cast<WNDPROC>(s_OverlayedOriginalProcedure), handle, message, wParam, lParam);
    }

	std::string RegisterPlugin(IDrawing* plugin)
	{
		std::string UUID = generate_uuid();
		s_Plugins.insert(make_pair(UUID, plugin));

		return UUID;
	}

	bool RemovePlugin(std::string UUID)
	{
		auto it = s_Plugins.find(UUID);

		if (it != s_Plugins.end())
		{
			s_Plugins.erase(UUID);
			return true;
		}
		return false;
	}

    std::mutex* GetMutex()
    {
        return &s_Mutex;
    }

    void SendVertices(std::vector<Vertices> vertices)
    {
        // Just to make sure we don't bug the code by sending multiple vertices at the same time. :)
        s_Mutex.lock();
        // TODO: move vertices to list!
        s_Mutex.unlock();
    }

}