#pragma once

#include <windows.h>

#include "renderer.h"

// Font Data needed for all widgets
#include "thirdparty/IconFontcppHeaders/IconsForkAwesome.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ImFont;

namespace raid {
namespace sandbox {

class ImGuiFramework
{
public:

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
	void Update();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

private:

	void InitFonts();
	ImFont* DefaultFont;
	ImFont* FontAwesome;
};

} // namespace sandbox
} // namespace raid