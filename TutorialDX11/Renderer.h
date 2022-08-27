#pragma once
#include "Window.h"
#include <d3d11.h>

class Renderer {
public:
	Renderer(Window& window);
	void beginFrame();			// Clear frame at start
	void endFrame();			// Swap buffer
	void CleanUp(void);
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

private:
	void createDevice(Window& window);
	void createRenderTarget();

	IDXGISwapChain* m_swapChain = nullptr;			// Swap buffer at end of frame
	ID3D11Device* m_device = nullptr;				// Used to create resources
	ID3D11DeviceContext* m_deviceContext = nullptr;	// Used to use resources and render

	// Render target
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC m_backBufferDesc;
};