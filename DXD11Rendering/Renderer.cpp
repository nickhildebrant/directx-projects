#include "Renderer.h"
#include "DirectXErrors.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib") // compiler sets linker settings to allow D3D11CreateDeviceAndSwapChain
#pragma comment(lib, "D3DCompiler.lib")

// graphics exception macros
#define GFX_EXCEPT_NOINFO(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED( hr = (hrcall))) throw Renderer::GraphicsHrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if(FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Renderer::InfoException(__LINE__, __FILE__, v);}}
#else
#define GFX_EXCEPT(hr) Renderer::GraphicsHrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

Renderer::Renderer(HWND handle)
{
	CreateDevice(handle);
	CreateRenderTarget();
	CreateDepthStencil();
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

void Renderer::DrawTestTriangle(float angle, float x, float z)
{
	HRESULT hr;

	struct Vertex
	{
		struct {
			float x;
			float y;
			float z;
		} position;

		/*struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;*/
	};

	// Vertices for the triangle
	Vertex vertices[] =
	{
		{ -1.0f,	-1.0f,	-1.0f },
		{  1.0f,	-1.0f,	-1.0f },
		{ -1.0f,	 1.0f,	-1.0f },
		{  1.0f,	 1.0f,	-1.0f },
		{ -1.0f,	-1.0f,	 1.0f },
		{  1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	 1.0f,	 1.0f },
		{  1.0f,	 1.0f,	 1.0f },
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	GFX_THROW_INFO(m_device->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer));

	// Binding vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// create the index buffer
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.StructureByteStride = sizeof(unsigned short);
	
	D3D11_SUBRESOURCE_DATA indexSubresourcesData = {};
	indexSubresourcesData.pSysMem = indices;

	GFX_THROW_INFO(m_device->CreateBuffer(&indexBufferDescription, &indexSubresourcesData, &indexBuffer));

	// bind index buffer
	m_deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// create a constant buffer for a transformation matrix
	struct ConstantBuffer {
		DirectX::XMMATRIX transformMatrix;
	};

	float aspectRatio = 3.0f / 4.0f;
	const ConstantBuffer matrixBuffer =
	{
		// Z-rotation matrix
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationX(angle) *
				DirectX::XMMatrixRotationZ(angle) *
				DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, aspectRatio, 0.1f, 100.0f)
			)
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	D3D11_BUFFER_DESC constantBufferDescription = {};
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescription.MiscFlags = 0u;
	constantBufferDescription.ByteWidth = sizeof(matrixBuffer);
	constantBufferDescription.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constantSubresourcesData = {};
	constantSubresourcesData.pSysMem = &matrixBuffer;

	GFX_THROW_INFO(m_device->CreateBuffer(&constantBufferDescription, &constantSubresourcesData, &constantBuffer));

	// bind constant buffer to the vertex shader
	m_deviceContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

	// Creating colors for the face of the cube
	// lookup table for cube face colors
	struct ColorConstantBuffer {
		struct {
			float r;
			float g;
			float b;
			float a;
		} faceColors[6];
	};

	const ColorConstantBuffer colorBuffer = // colorBuffer = 
	{
		{
			{ 1.0f,	0.0f, 1.0f },
			{ 1.0f,	0.0f, 0.0f },
			{ 0.0f,	1.0f, 0.0f },
			{ 0.0f,	0.0f, 1.0f },
			{ 1.0f,	1.0f, 0.0f },
			{ 0.0f,	1.0f, 1.0f },
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> colorConstantBuffer;
	D3D11_BUFFER_DESC colorBufferDescription;
	colorBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	colorBufferDescription.CPUAccessFlags = 0u;
	colorBufferDescription.MiscFlags = 0u;
	colorBufferDescription.ByteWidth = sizeof(colorBuffer);
	colorBufferDescription.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA colorSubresource = {};
	colorSubresource.pSysMem = &colorBuffer;

	GFX_THROW_INFO(m_device->CreateBuffer(&colorBufferDescription, &colorSubresource, &colorConstantBuffer));

	// bind constant buffer to the pixel shader
	m_deviceContext->PSSetConstantBuffers(0u, 1u, colorConstantBuffer.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// Creating the pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"PixelShader.cso", &blob));
	GFX_THROW_INFO_ONLY(m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

	// bind pixel shader
	m_deviceContext->PSSetShader(pixelShader.Get(), 0, 0);

	// Creating the vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO_ONLY(D3DReadFileToBlob(L"VertexShader.cso", &blob));
	GFX_THROW_INFO_ONLY(m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));

	// bind vertex shader
	m_deviceContext->VSSetShader(vertexShader.Get(), 0, 0);

	// input layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC elementDesc[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(m_device->CreateInputLayout(elementDesc, (UINT)std::size(elementDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

	// bind input layout
	m_deviceContext->IASetInputLayout(inputLayout.Get());

	// output merger binding done in BeginFrame()
	m_deviceContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), nullptr);// m_depthStencilView.Get());

	// set primitive topology to triangle list
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = 640;
	viewPort.Height = 480;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &viewPort);

	GFX_THROW_INFO_ONLY(m_deviceContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
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