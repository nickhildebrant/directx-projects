#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Renderer& renderer, const Drawable& parent, UINT slot) : parent(parent)
{
	if ( !pVertexConstantBuffer ) pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( renderer, slot );
}

void TransformConstantBuffer::Bind(Renderer& renderer) noexcept
{
	const auto modelView = parent.GetTransformXM() * renderer.GetView();

	const Transforms transforms =
	{
		DirectX::XMMatrixTranspose( modelView ),
		DirectX::XMMatrixTranspose( modelView * renderer.GetProjection() )
	};

	pVertexConstantBuffer->Update(renderer, transforms );
	pVertexConstantBuffer->Bind(renderer);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVertexConstantBuffer;