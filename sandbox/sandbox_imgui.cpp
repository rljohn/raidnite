#include "sandbox/pch.h"
#include "sandbox_imgui.h"

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_win32.h"
#include "thirdparty/imgui/imgui_impl_dx11.h"
#include "thirdparty/imgui/imgui_internal.h"
#include "thirdparty/imgui/ForkAwesome.h"

#include "thirdparty/imgui/imgui_demo.cpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace raid {
namespace sandbox {

LRESULT ImGuiFramework::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return 0;
}

void ImGuiFramework::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, context);

	InitFonts();
}

void ImGuiFramework::InitFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	const int fontSize = 13.0f;
	const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.GlyphMinAdvanceX = fontSize;
	icons_config.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryCompressedBase85TTF(imgui::fonts::GetForkAwesomeData(), fontSize, &icons_config, icons_ranges);
	io.Fonts->Build();
}

void ImGuiFramework::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool showDemo = false;
	ImGui::ShowDemoWindow(&showDemo);
}

void ImGuiFramework::BeginFrame()
{
	ImGui::Render();
}

void ImGuiFramework::EndFrame()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiFramework::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

} // namespace sandbox
} // namespace raid