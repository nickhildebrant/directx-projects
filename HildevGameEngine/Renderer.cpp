#include "Renderer.h"
//#include "ImGUI/imgui_impl_dx11.h"
//#include "ImGUI/imgui_impl_win32.h"
#include <sstream>

Renderer::Renderer( HWND handle, int width, int height ) : m_width( width ), m_height( height )
{
	CreateDevice( handle );
	CreateRenderTarget();
	CreateDepthStencil();

	// binding the render target and depth stencil to the output merger
	pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), pDepthStencil.Get() );

	// configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = m_width;
	viewPort.Height = m_height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pContext->RSSetViewports( 1, &viewPort );

	// init GUI
	//ImGui_ImplDX11_Init( pDevice.Get(), pContext.Get() );
}

Renderer::~Renderer()
{
	//ImGui_ImplDX11_Shutdown();
}

void Renderer::CreateDevice( HWND handle )
{
	// Define the swap chain and clear out struct
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	swapChainDesc.BufferCount = 1;									// Double buffering = 1, Triple = 2 (# of back buffers)
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 32-bit Color, 8 per channel
	swapChainDesc.BufferDesc.Width = 0;								// Looks at graphics adapter for width
	swapChainDesc.BufferDesc.Height = 0;							// Looks at graphics adapter for height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how the swapchain is to be used
	swapChainDesc.OutputWindow = handle;							// window to be used
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			// How the swap is handled, best in most cases
	swapChainDesc.SampleDesc.Count = 1;								// Anti-Aliasing sampling, currently using 1x
	swapChainDesc.Windowed = TRUE;									// Windowed, false = fullscreen
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// Allow full-screen switching

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef NDEBUG
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create the swap chain device and device context
	HRESULT hr;// error handling
	D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0,
									D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, nullptr, &pContext );
}

void Renderer::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	HRESULT hr;// error handling

	pSwapChain->GetBuffer( 0, __uuidof( ID3D11Resource ), &backBuffer );
	pDevice->CreateRenderTargetView( backBuffer.Get(), nullptr, &pRenderTargetView );
}

void Renderer::CreateDepthStencil()
{
	HRESULT hr; // error handling

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthDescription = {};
	depthDescription.DepthEnable = TRUE;
	depthDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDescription.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;

	pDevice->CreateDepthStencilState( &depthDescription, &depthState );

	// bind depth stencil to output merger
	pContext->OMSetDepthStencilState( depthState.Get(), 1 );

	// create the depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthTextureDescription = {};
	depthTextureDescription.Width = m_width;
	depthTextureDescription.Height = m_height;
	depthTextureDescription.MipLevels = 1u;
	depthTextureDescription.ArraySize = 1u;
	depthTextureDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDescription.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDescription.SampleDesc.Count = 1u;
	depthTextureDescription.SampleDesc.Quality = 0u;

	pDevice->CreateTexture2D( &depthTextureDescription, nullptr, &depthStencilTexture );

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDescription = {};
	stencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDescription.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView( depthStencilTexture.Get(), &stencilViewDescription, &pDepthStencil );
}

void Renderer::EnableUI() noexcept { uiEnabled = true; }
void Renderer::DisableUI() noexcept { uiEnabled = false; }
bool Renderer::IsUIEnabled() const noexcept { return uiEnabled; }

void Renderer::BeginFrame( float r, float g, float b, float a )
{
	// update UI frame
	if ( uiEnabled )
	{
		/*ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();*/
	}

	// binding of render target and depth stencil to Output Merger
	pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), pDepthStencil.Get() );

	const float color[] = { r, g, b, a };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(), color );
	pContext->ClearDepthStencilView( pDepthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
}

void Renderer::EndFrame()
{
	// update UI frame
	if ( uiEnabled )
	{
		/*ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );*/
	}

	// Swapping buffer
	HRESULT hr;
	if(FAILED(hr = pSwapChain->Present( 1u, 0u ) ) )// First param is V-sync, second is flag
	{
		if ( hr == DXGI_ERROR_DEVICE_REMOVED ) throw ( pDevice->GetDeviceRemovedReason() );
	}
}

void Renderer::DrawIndexed( UINT count )
{
	pContext->DrawIndexed( count, 0u, 0u );
}

void Renderer::SetCameraView( DirectX::FXMMATRIX view )
{
	viewMatrix = view;
}

DirectX::XMMATRIX Renderer::GetView() const
{
	return viewMatrix;
}

void Renderer::SetProjection( DirectX::FXMMATRIX projection )
{
	projectionMatrix = projection;
}

DirectX::XMMATRIX Renderer::GetProjection() const
{
	return projectionMatrix;
}