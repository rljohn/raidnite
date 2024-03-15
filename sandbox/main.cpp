#include "sandbox/pch.h"

#include <Windows.h>
#include <chrono>
#include <thread>

#include "window.h"
#include "sandbox_game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    raid::sandbox::Window window;
    if (!window.Init(hInstance, nCmdShow, 1440, 900))
    {
        return false;
    }

    raid::sandbox::GameSandbox game;
    game.Init();
    
    // Cap to 120 FPS for the sandbox
    const std::chrono::nanoseconds frameTime(8333333);
    std::chrono::steady_clock::time_point nextFrame = std::chrono::steady_clock::now() + frameTime;
    
    MSG msg = { 0 };
    while (true) 
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        window.Update();
        game.Update();
        window.Render();

        // Cap the frame rate
        std::this_thread::sleep_until(nextFrame);
        nextFrame += frameTime;
    }

    game.Shutdown();
    window.Shutdown();

    return (int)msg.wParam;
}

