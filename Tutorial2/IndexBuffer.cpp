#include "IndexBuffer.h"
#include "RendererErrorMacros.h"

IndexBuffer::IndexBuffer(Renderer& renderer, const std::vector<unsigned short>& indices) : count((UINT)indices.size())
{
	INFO_MANAGER(renderer);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&ibd, &isd, &m_indexBuffer));
}

void IndexBuffer::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}