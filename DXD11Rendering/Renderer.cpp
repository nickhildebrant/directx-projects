#include "Renderer.h"
#include "DirectXErrors.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib") // compiler sets linker settings to allow D3D11CreateDeviceAndSwapChain

// graphics exception macros
#define GFX_EXCEPT_NOINFO(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED( hr = (hrcall))) throw Renderer::GraphicsHrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#else
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr))
#endif

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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create the swap chain device and device context
	HRESULT hr;// error handling
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext));
}

void Renderer::CreateRenderTarget()
{
	ID3D11Resource* backBuffer;
	HRESULT hr;// error handling
	GFX_THROW_INFO(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&backBuffer));
	GFX_THROW_INFO(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView));

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

void Renderer::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

// Graphics Exceptions *********************************************************************************************
Renderer::GraphicsHrException::GraphicsHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: ExceptionHandler(line, file), m_hresult(hr)
{
	// combine messages with newlines into one string
	for (const auto& str : infoMsgs)
	{
		m_info += str;
		m_info.push_back('\n');
	}

	// remove last newline
	if (m_info.empty()) m_info.pop_back();
}

const char* Renderer::GraphicsHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Desc: " << GetErrorDescription() << std::endl
		<< GetOriginString();

	if (m_info.empty()) oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;

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