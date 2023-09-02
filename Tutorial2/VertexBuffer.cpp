#include "VertexBuffer.h"

VertexBuffer::VertexBuffer( Renderer& renderer, const VertexHandler::VertexBuffer& vertexBuffer ) : stride( (UINT) vertexBuffer.GetLayout().Size() )
{
	INFO_MANAGER( renderer );

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = UINT( vertexBuffer.SizeBytes() );
	bufferDesc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertexBuffer.GetData();

	GFX_THROW_INFO( GetDevice( renderer )->CreateBuffer( &bufferDesc, &subresourceData, &pVertexBuffer ) );
}

void VertexBuffer::Bind(Renderer& renderer) noexcept
{
	const UINT offset = 0u;
	GetContext(renderer)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}