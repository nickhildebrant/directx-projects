#include "VertexBuffer.h"

void VertexBuffer::Bind( Renderer& renderer )
{
	const UINT offset = 0u;
	renderer.GetDeviceContext()->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
}