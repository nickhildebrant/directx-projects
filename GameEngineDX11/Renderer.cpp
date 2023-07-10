#include "Renderer.h"

Renderer::Renderer( Window& window )
{
	CreateDevice( window );
	CreateRenderTarget();
	CreateDepthStencil();

	// binding the render target and depth stencil to the output merger
	m_deviceContext->OMSetRenderTargets( 1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );

	// configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = 640;
	viewPort.Height = 480;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports( 1, &viewPort );
}

void Renderer::CreateDevice( Window& window )
{
	// Define the swap chain and clear out struct
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferCount = 1u;									// Double buffering = 1, Triple = 2 (# of back buffers)
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 32-bit Color, 8 per channel
	swapChainDesc.BufferDesc.Width = window.getWidth();
	swapChainDesc.BufferDesc.Height = window.getHeight();
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how the swapchain is to be used
	swapChainDesc.OutputWindow = window.getHandle();				// window to be used
	swapChainDesc.SampleDesc.Count = 1u;							// Anti-Aliasing, currently using 4x
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

void Renderer::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;

	m_swapChain->GetBuffer( 0, __uuidof( ID3D11Resource ), &backBuffer );
	m_device->CreateRenderTargetView( backBuffer.Get(), nullptr, &m_renderTargetView );
}

void Renderer::CreateDepthStencil()
{
	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthDescription = {};
	depthDescription.DepthEnable = TRUE;
	depthDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDescription.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;

	m_device->CreateDepthStencilState( &depthDescription, &depthState );

	// bind depth stencil to output merger
	m_deviceContext->OMSetDepthStencilState( depthState.Get(), 1 );

	// create the depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthTextureDescription = {};
	depthTextureDescription.Width = 640u;
	depthTextureDescription.Height = 480u;
	depthTextureDescription.MipLevels = 1u;
	depthTextureDescription.ArraySize = 1u;
	depthTextureDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDescription.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDescription.SampleDesc.Count = 1u;
	depthTextureDescription.SampleDesc.Quality = 0u;

	m_device->CreateTexture2D( &depthTextureDescription, nullptr, &depthStencilTexture );

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDescription = {};
	stencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDescription.Texture2D.MipSlice = 0u;

	m_device->CreateDepthStencilView( depthStencilTexture.Get(), &stencilViewDescription, &m_depthStencilView );
}

void Renderer::BeginFrame()
{
	// binding of render target and depth stencil to Output Merger
	m_deviceContext->OMSetRenderTargets( 1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );

	// Setting the background color
	ClearBuffer( 0.0f, 0.2f, 0.4f, 1 );
}

void Renderer::EndFrame()
{
	// Swapping buffer
	m_swapChain->Present( 1, 0 );	// First param is V-sync, second is flag
}

void Renderer::ClearBuffer( float r, float g, float b, float a )
{
	const float color[] = { r, g, b , a };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView.Get(), color );
	m_deviceContext->ClearDepthStencilView( m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
}

void Renderer::DrawIndexed( UINT count ) { m_deviceContext->DrawIndexed( count, 0u, 0u ); }

void Renderer::SetProjection( DirectX::FXMMATRIX projection ) { m_projection = projection; }
DirectX::XMMATRIX Renderer::GetProjection() { return m_projection; }