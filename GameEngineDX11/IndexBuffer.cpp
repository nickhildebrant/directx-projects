#include "IndexBuffer.h"

IndexBuffer::IndexBuffer( Renderer& renderer, const std::vector<unsigned short>& indices ) : count( (UINT) indices.size() )
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = UINT( count * sizeof( unsigned short ) );
	indexBufferDesc.StructureByteStride = sizeof( unsigned short );

	D3D11_SUBRESOURCE_DATA indexSubresource = {};
	indexSubresource.pSysMem = indices.data();

	renderer.GetDevice()->CreateBuffer( &indexBufferDesc, &indexSubresource, &pIndexBuffer );
}

void IndexBuffer::Bind( Renderer& renderer )
{
	renderer.GetDeviceContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const
{
	return count;
}