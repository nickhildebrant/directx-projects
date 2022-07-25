#pragma once
#include "Window.h"
#include <d3d11.h>

class Renderer {
public:
	Renderer(Window& window);

private:
	IDXGISwapChain* m_SwapChain = nullptr;			// Swap buffer at end of frame
	ID3D11Device* m_device = nullptr;				// Used to create resources
	ID3D11DeviceContext* m_deviceContext = nullptr;	// Used to use resources and render
};