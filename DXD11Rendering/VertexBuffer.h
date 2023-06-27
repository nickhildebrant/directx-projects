#pragma once
#include "Bindable.h"
#include "RendererErrorMacros.h"

class VertexBuffer : public Bindable {
public:
	template<class V>
	VertexBuffer(Renderer& renderer, const std::vector<V>& vertices) : stride(sizeof(V))
	{
		INFO_MANAGER(renderer);

		D3D11_BUFFER_DESC bufferDescription = {};
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.CPUAccessFlags = 0u;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = UINT(sizeof(V) * vertices.size());
		bufferDescription.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data();

		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&bufferDescription, &subresourceData, &m_vertexBuffer));
	}
	void Bind(Renderer& renderer) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};