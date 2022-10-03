#include "Renderer.h"

Renderer::Renderer(HWND handle)
{
	CreateDevice(handle);
	CreateRenderTarget();
}

Renderer::~Renderer()
{
	if (m_deviceContext) m_deviceContext->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_device) m_device->Release();
	if (m_renderTargetView) m_renderTargetView->Release();
}

void Renderer::CreateDevice(HWND handle)
{
	// Define the swap chain and clear out struct
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;									// Double buffering = 1, Triple = 2 (# of back buffers)
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 32-bit Color, 8 per channel
	swapChainDesc.BufferDesc.Width = 0;								// Looks at graphics adapter for width
	swapChainDesc.BufferDesc.Height = 0;							// Looks at graphics adapter for height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how the swapchain is to be used
	swapChainDesc.OutputWindow = handle;							// window to be used
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// How the swap is handled, best in most cases
	swapChainDesc.SampleDesc.Count = 4;								// Anti-Aliasing, currently using 4x
	swapChainDesc.Windowed = TRUE;									// Windowed, false = fullscreen
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// Allow full-screen switching

	// Create the swap chain device and device context
	auto result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);

	// Error handling
	if (result != S_OK)
	{
		MessageBox(nullptr, "Error with DX11: " + result, "Error", MB_OK);
		exit(0);
	}
}

void Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);

	backBuffer->GetDesc(&m_backBufferDesc);
	backBuffer->Release();
}