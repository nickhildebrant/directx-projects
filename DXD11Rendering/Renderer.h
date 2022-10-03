#pragma once
#include <Windows.h>
#include <d3d11.h>

class Renderer {
public:
	Renderer(HWND handle);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();

	void BeginFrame();			// Clear frame at start
	void EndFrame();			// Swap buffer

	void ClearBuffer(float r, float g, float b) {
		const float color[] = { r, g, b };
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	}

private:
	void CreateDevice(HWND handle);
	void CreateRenderTarget();

	ID3D11Device* m_device = nullptr;				// Used to create resources
	IDXGISwapChain* m_swapChain = nullptr;			// Swap buffer at end of frame
	ID3D11DeviceContext* m_deviceContext = nullptr;	// Used to use resources and render

	// Render target
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	//D3D11_Resource_desc m_backBufferDesc;
};