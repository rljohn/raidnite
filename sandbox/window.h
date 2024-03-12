#pragma once

#include <Windows.h>

#include "renderer.h"
#include "sandbox_imgui.h"

namespace raid {
namespace sandbox {

class Window
{
public:

	Window()
		: m_ImGui()
	{
	}

	bool Init(HINSTANCE hInstance, const int nCmdShow, const int width, const int height);
	void Update();
	void Render();
	void Shutdown();

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	HWND m_Hwnd = 0;
	WNDCLASSW m_Wc = { 0 };
	Renderer m_Renderer;
	ImGuiFramework m_ImGui;

	int m_Width = 0;
	int m_Height = 0;
	bool m_Resize = false;
};

} // namespace sandbox
} // namespace raid