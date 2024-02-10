#include <Windows.h>
#include <chrono>
#include <thread>

#include "window.h"
#include "game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    raid::sandbox::Window window;
    if (!window.Init(hInstance, nCmdShow, 1280, 720))
    {
        return false;
    }

    raid::sandbox::Game game;
    game.Init();
    
    // Cap to 60 FPS for the sandbox
    const std::chrono::nanoseconds frameTime(16666666);
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

