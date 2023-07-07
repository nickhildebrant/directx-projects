#include "Renderer.h"

Renderer::Renderer( Window& window )
{
	createDevice( window );
	createRenderTarget();
}

void Renderer::createDevice( Window& window )
{
	// Define the swap chain and clear out struct
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferCount = 1;									// Double buffering = 1, Triple = 2 (# of back buffers)
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 32-bit Color, 8 per channel
	swapChainDesc.BufferDesc.Width = window.getWidth();
	swapChainDesc.BufferDesc.Height = window.getHeight();
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how the swapchain is to be used
	swapChainDesc.OutputWindow = window.getHandle();				// window to be used
	swapChainDesc.SampleDesc.Count = 4;								// Anti-Aliasing, currently using 4x
	swapChainDesc.Windowed = true;									// Windowed, false = fullscreen
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// Allow full-screen switching

	// Create the swap chain device and device context
	auto result = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext );

	// Error handling
	if ( result != S_OK )
	{
		MessageBox( nullptr, "Error with DX11: " + result, "Error", MB_OK );
		exit( 0 );
	}
}

void Renderer::createRenderTarget()
{
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*) &backBuffer );
	m_device->CreateRenderTargetView( backBuffer, nullptr, &m_renderTargetView );

	backBuffer->GetDesc( &m_backBufferDesc );
	backBuffer->Release();
}

void Renderer::beginFrame()
{
	// Bind render target to output merger
	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, nullptr );

	// Set viewport
	auto viewport = CD3D11_VIEWPORT( 0.f, 0.f, (float) m_backBufferDesc.Width, (float) m_backBufferDesc.Height );
	m_deviceContext->RSSetViewports( 1, &viewport );

	// Setting the background color
	float clearColor[] = { 0.0f, 0.2f, 0.4f, 1 }; // RGBA
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, clearColor );
}

void Renderer::endFrame()
{
	// Swapping buffer
	m_swapChain->Present( 1, 0 );	// First param is V-sync, second is flag
}

void Renderer::CleanUp( void )
{
	// switches to windowed
	m_swapChain->SetFullscreenState( FALSE, nullptr );

	m_swapChain->Release();
	m_renderTargetView->Release();
	m_device->Release();
	m_deviceContext->Release();
}

ID3D11Device* Renderer::getDevice() { return m_device; }

ID3D11DeviceContext* Renderer::getDeviceContext() { return m_deviceContext; }