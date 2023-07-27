#pragma once
#include "Bindable.h"
#include "RendererErrorMacros.h"

template<typename C>
class ConstantBuffer : public Bindable {
public:
	void Update(Renderer& renderer, const C& consts)
	{
		INFO_MANAGER(renderer);

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		GFX_THROW_INFO(GetContext(renderer)->Map(m_constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubResource));
		memcpy(mappedSubResource.pData, &consts, sizeof(consts));
		GetContext(renderer)->Unmap(m_constantBuffer.Get(), 0u);
	}

	ConstantBuffer(Renderer& renderer, const C& consts, UINT slot = 0 ) : slot( slot )
	{
		INFO_MANAGER(renderer);

		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = sizeof(consts);
		bufferDescription.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&bufferDescription, &subresourceData, &m_constantBuffer));
	}

	ConstantBuffer(Renderer& renderer, UINT slot = 0) : slot(slot)
	{
		INFO_MANAGER(renderer);

		D3D11_BUFFER_DESC bufferDescription;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0u;
		bufferDescription.ByteWidth = sizeof(C);
		bufferDescription.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&bufferDescription, nullptr, &m_constantBuffer));
	}
protected:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::slot;
	using ConstantBuffer<C>::m_constantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Renderer& renderer) noexcept override
	{
		GetContext(renderer)->VSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::slot;
	using ConstantBuffer<C>::m_constantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Renderer& renderer) noexcept override
	{
		GetContext(renderer)->PSSetConstantBuffers(slot, 1u, m_constantBuffer.GetAddressOf());
	}
};