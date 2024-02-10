
#include "window.h"

namespace raid {
namespace sandbox {


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (pWindow) 
    {
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_ImGui.WindowProc(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            return 0;
		}

        // Queue resize
        m_Width = (UINT)LOWORD(lParam);
        m_Height = (UINT)HIWORD(lParam);
        m_Resize = true;
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    }

    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Window::Init(HINSTANCE hInstance, const int nCmdShow, const int width, const int height)
{
    const wchar_t CLASS_NAME[] = L"RaidniteSandboxWnd";
    const wchar_t* TITLE = L"Raidnite Sandbox";

    m_Wc = { 0 };
    m_Wc.lpfnWndProc = WndProc;
    m_Wc.hInstance = hInstance;
    m_Wc.lpszClassName = CLASS_NAME;

    m_Width = width;
    m_Height = height;

    ::RegisterClass(&m_Wc);

    m_Hwnd = ::CreateWindowExW(0, 
        CLASS_NAME, TITLE, 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        m_Width, m_Height, nullptr, nullptr, hInstance, nullptr);

    if (m_Hwnd == nullptr)
        return false;

    SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    if (!m_Renderer.Init(m_Hwnd))
    {
        m_Renderer.Shutdown();
        ::UnregisterClassW(m_Wc.lpszClassName, m_Wc.hInstance);
        return false;
    }

    ::ShowWindow(m_Hwnd, nCmdShow);
    ::UpdateWindow(m_Hwnd);

    m_ImGui.Init(m_Hwnd, m_Renderer.GetDevice(), m_Renderer.GetDeviceContext());

    return true;
}

void Window::Update()
{
    if (m_Resize)
    {
        m_Renderer.Resize(m_Width, m_Height);
    }

    m_ImGui.Update();
}

void Window::Render()
{
    m_ImGui.BeginFrame();
    m_Renderer.BeginFrame();

    m_ImGui.EndFrame();
    m_Renderer.EndFrame();
}

void Window::Shutdown()
{
    m_ImGui.Shutdown();
    m_Renderer.Shutdown();
    ::DestroyWindow(m_Hwnd);
    ::UnregisterClassW(m_Wc.lpszClassName, m_Wc.hInstance);
}

} // namespace sandbox
} // namespace raid