#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Renderer& renderer, const Drawable& parent) :
	vertexConstantBuffer(renderer), parent(parent)
{

}

void TransformConstantBuffer::Bind(Renderer& renderer) noexcept
{
	vertexConstantBuffer.Update(renderer, DirectX::XMMatrixTranspose(parent.GetTransformXM() * renderer.GetView() * renderer.GetProjection()));
	vertexConstantBuffer.Bind(renderer);
}