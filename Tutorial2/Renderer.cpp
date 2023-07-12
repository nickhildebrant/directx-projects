#include "Renderer.h"
#include "RendererErrorMacros.h"
#include <sstream>

Renderer::Renderer(HWND handle)
{
	CreateDevice(handle);
	CreateRenderTarget();
	CreateDepthStencil();

	// binding the render target and depth stencil to the output merger
	m_deviceContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = 640;
	viewPort.Height = 480;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &viewPort);
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
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext));
}

void Renderer::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	HRESULT hr;// error handling

	GFX_THROW_INFO(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_INFO(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));
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

	GFX_THROW_INFO(m_device->CreateDepthStencilState(&depthDescription, &depthState));

	// bind depth stencil to output merger
	m_deviceContext->OMSetDepthStencilState(depthState.Get(), 1);

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

	GFX_THROW_INFO(m_device->CreateTexture2D(&depthTextureDescription, nullptr, &depthStencilTexture));

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDescription = {};
	stencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	stencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilViewDescription.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(m_device->CreateDepthStencilView(depthStencilTexture.Get(), &stencilViewDescription, &m_depthStencilView));
}

void Renderer::BeginFrame()
{
	// binding of render target and depth stencil to Output Merger
	m_deviceContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set viewport
	//auto viewport = CD3D11_VIEWPORT(0.f, 0.f, (float)m_backBufferDesc.Width, (float)m_backBufferDesc.Height);
	//m_deviceContext->RSSetViewports(1, &viewport);

	// Setting the background color
	//const float color[]{ 0.0f, 0.2f, 0.4f, 1.0f };
	//m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	ClearBuffer(0.0f, 0.2f, 0.4f, 1.0f);
}

void Renderer::EndFrame()
{
#ifndef NDEBUG
	infoManager.Set();
#endif

	// Swapping buffer
	HRESULT hr;
	if (FAILED(hr = m_swapChain->Present(1u, 0u))) // First param is V-sync, second is flag
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED) throw GFX_DEVICE_REMOVED_EXCEPT(m_device->GetDeviceRemovedReason());
		else throw GFX_EXCEPT(hr);
	}
}

void Renderer::ClearBuffer(float r, float g, float b, float a)
{
	const float color[] = { r, g, b , a };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Renderer::DrawIndexed(UINT count)
{
	GFX_THROW_INFO_ONLY(m_deviceContext->DrawIndexed(count, 0u, 0u));
}

void Renderer::SetProjection(DirectX::FXMMATRIX projection) noexcept
{
	m_projection = projection;
}

DirectX::XMMATRIX Renderer::GetProjection() const noexcept
{
	return m_projection;
}

// Graphics Exceptions *********************************************************************************************
Renderer::GraphicsHrException::GraphicsHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: GraphicsException(line, file), m_hresult(hr)
{
	// combine messages with newlines into one string
	for (const auto& str : infoMsgs)
	{
		m_info += str;
		m_info.push_back('\n');
	}

	// remove last newline
	if (!m_info.empty()) m_info.pop_back();
}

const char* Renderer::GraphicsHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Desc: " << GetErrorDescription() << std::endl;

	if (!m_info.empty()) oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;

	oss << GetOriginString();
	_buffer = oss.str();
	return _buffer.c_str();
}

const char* Renderer::GraphicsHrException::GetType() const noexcept{ return "Graphics Renderer Exception"; }

HRESULT Renderer::GraphicsHrException::GetErrorCode() const noexcept { return m_hresult; }

std::string Renderer::GraphicsHrException::GetErrorString() const noexcept { return DXGetErrorString(m_hresult); }

std::string Renderer::GraphicsHrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_hresult, buf, sizeof(buf));
	return buf;
}

std::string Renderer::GraphicsHrException::GetErrorInfo() const noexcept { return m_info; }

const char* Renderer::DeviceRemovedException::GetType() const noexcept { return "Graphics Render Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)"; }

Renderer::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	: ExceptionHandler(line, file)
{
	for (const auto& m : infoMsgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}

	// Removes final newline char
	if (!m_info.empty()) m_info.pop_back();
}

const char* Renderer::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << "\nInfo: " << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	_buffer = oss.str();
	return _buffer.c_str();
}

const char* Renderer::InfoException::GetType() const noexcept { return "Graphics Renderer Info Exception"; }

std::string Renderer::InfoException::GetErrorInfo() const noexcept { return m_info; }