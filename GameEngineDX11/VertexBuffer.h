#pragma once
#include "Bindable.h"

class VertexBuffer : public Bindable {
public:
	template<class V>
	VertexBuffer( Renderer& renderer, const std::vector<V>& vertices ) : stride( sizeof( V ) )
	{
		D3D11_BUFFER_DESC bufferDescription = {};
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.CPUAccessFlags = 0u;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = UINT( sizeof( V ) * vertices.size() );
		bufferDescription.StructureByteStride = sizeof( V );

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data();

		renderer.GetDevice()->CreateBuffer(&bufferDescription, &subresourceData, &pVertexBuffer) );
	}

	void Bind( Renderer& renderer ) override
	{
		const UINT offset = 0u;
		renderer.GetDeviceContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};