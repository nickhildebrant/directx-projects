#pragma once
#include "Window.h"
#include <d3d11.h>

class Renderer {
public:
	Renderer(Window& window);

private:
	void CreateDevice(Window& window);
	void CreateRenderTarget();

	ID3D11Device* m_device = nullptr;				// Used to create resources
	IDXGISwapChain* m_swapChain = nullptr;			// Swap buffer at end of frame
	ID3D11DeviceContext* m_deviceContext = nullptr;	// Used to use resources and render
};