#include "Renderer.h"

#pragma comment(lib, "d3d11.lib") // compiler sets linker settings to allow D3D11CreateDeviceAndSwapChain

Renderer::Renderer(HWND handle)
{
	CreateDevice(handle);
	CreateRenderTarget();
}

Renderer::~Renderer()
{
	if (m_renderTargetView) m_renderTargetView->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_device) m_device->Release();
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
	auto result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);

	// Error handling
	if (result != S_OK)
	{
		MessageBox(nullptr, "Error with DX11(CreateDeviceAndSwapChain): " + result, "Error", MB_OK);
		exit(-1);
	}
}

void Renderer::CreateRenderTarget()
{
	ID3D11Resource* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&backBuffer);
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);

	backBuffer->Release();
}

void Renderer::BeginFrame()
{
	// Bind render target to output merger
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Set viewport
	//auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
	//m_deviceContext->RSSetViewports(1, &viewport);

	// Setting the background color
	float clearColor[] = { 0.0f, 0.2f, 0.4f, 1 }; // RGBA
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
}

void Renderer::EndFrame()
{
	// Swapping buffer
	auto result = m_swapChain->Present(1, 0);	// First param is V-sync, second is flag

	// Error handling
	if (result != S_OK)
	{
		MessageBox(nullptr, "" + m_device->GetDeviceRemovedReason() + result, "Error", MB_OK);
		exit(-1);
	}
}