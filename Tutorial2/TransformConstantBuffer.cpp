#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Renderer& renderer, const Drawable& parent, UINT slot) : parent(parent)
{
	if ( !pVertexConstantBuffer ) pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( renderer, slot );
}

void TransformConstantBuffer::Bind(Renderer& renderer) noexcept
{
	UpdateBindImpl( renderer, GetTransforms( renderer ) );
}

void TransformConstantBuffer::UpdateBindImpl( Renderer& renderer, const Transforms& tf )
{
	pVertexConstantBuffer->Update( renderer, tf );
	pVertexConstantBuffer->Bind( renderer );
}

TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransforms( Renderer& renderer )
{
	const auto modelView = parent.GetTransformXM() * renderer.GetView();

	return
	{
		DirectX::XMMatrixTranspose( modelView ),
		DirectX::XMMatrixTranspose( modelView * renderer.GetProjection() )
	};
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVertexConstantBuffer;