#pragma once

#include <windows.h>

#include "renderer.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

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
};

} // namespace sandbox
} // namespace raid