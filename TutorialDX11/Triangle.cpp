#include "Triangle.h"
#include <fstream>
#include <vector>

struct Vertex {
	float x, y;
	float r, g, b;
};

Triangle::Triangle(Renderer& renderer)
{
	createMesh(renderer);
	createShaders(renderer);
}

Triangle::~Triangle()
{
	m_vertexBuffer->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_inputLayout->Release();
}

void Triangle::draw(Renderer& renderer)
{
	auto deviceContext = renderer.getDeviceContext();

	// Bind triangle shaders
	deviceContext->IASetInputLayout(m_inputLayout);
	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	// Bind vertex buffer to render multiple times from memory
	UINT stride = sizeof(Vertex);	// Size between each vertex
	UINT offset = 0;				// Offset render for things like shadow
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);	// Hardware Specific

	// Draw
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// For devices without dedicated GPU
	deviceContext->Draw(3, 0);
}

void Triangle::createMesh(Renderer& renderer)
{
	// Defining vertices
	// Holds position and rgb color
	Vertex vertices[] = {
		{ -.5f, -.5f, 0, 0, 1 },
		{ 0, .5f, 1, 0, 0 },
		{ .5f, -.5f, 0, 1, 0 },
	};

	// Creating vertex buffer, putting verticies in VRAM
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);

	//vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// write access allowed by CPU and GPU
	//vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;			// size of 3 triangles
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// use as a vertex buffer
	//vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// allows CPU to write to buffer

	// ^ does above comments easily ^
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = vertices;
	auto result = renderer.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Error handling
	if (result != S_OK)
	{
		MessageBox(nullptr, "Error with DX11: " + result, "Error", MB_OK);
		exit(0);
	}
}

void Triangle::createShaders(Renderer& renderer)
{
	// Create shaders
	std::ifstream vsFile("triangleVertexShader.cso", std::ios::binary);
	std::ifstream psFile("trianglePixelShader.cso", std::ios::binary);

	std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	renderer.getDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_vertexShader);
	renderer.getDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pixelShader);

	// Create input layouts
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	renderer.getDevice()->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &m_inputLayout);
}