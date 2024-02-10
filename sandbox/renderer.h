#pragma once

#include <d3d11.h>

namespace raid {
namespace sandbox {
	
class Renderer
{
public:

	bool Init(HWND hwnd);
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void Resize(const int width, const int height);

	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }

private:

	bool CreateDeviceD3D(HWND hwnd);
	void CleanupDeviceD3D();

	void CreateRenderTarget();
	void CleanupRenderTarget();

	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_mainRenderTargetView = nullptr;
};

} // namespace sandbox
} // namespace raid