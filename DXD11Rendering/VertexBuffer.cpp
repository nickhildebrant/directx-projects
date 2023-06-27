#include "VertexBuffer.h"

void VertexBuffer::Bind(Renderer& renderer) noexcept
{
	const UINT offset = 0u;
	GetContext(renderer)->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);
}