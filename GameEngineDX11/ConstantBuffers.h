#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable {
public:
	void Update( Renderer& renderer, const C& consts )
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		renderer.GetDeviceContext()->Map( pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource );
		memcpy( mappedSubResource.pData, &consts, sizeof( consts ) );
		renderer.GetDeviceContext()->Unmap( pConstantBuffer.Get(), 0u );
	}

	ConstantBuffer( Renderer& renderer, const C& consts )
	{
		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = sizeof( consts );
		bufferDescription.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = &consts;
		renderer.GetDevice()->CreateBuffer( &bufferDescription, &subresourceData, &pConstantBuffer );
	}

	ConstantBuffer( Renderer& renderer )
	{
		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = sizeof( C );
		bufferDescription.StructureByteStride = 0u;
		renderer.GetDevice()->CreateBuffer( &bufferDescription, nullptr, &pConstantBuffer );
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind( Renderer& renderer ) override
	{
		renderer.GetDeviceContext()->VSSetConstantBuffers( 0u, 1u, pConstantBuffer.GetAddressOf() );
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind( Renderer& renderer ) override
	{
		renderer.GetDeviceContext()->PSSetConstantBuffers( 0u, 1u, pConstantBuffer.GetAddressOf() );
	}
};
