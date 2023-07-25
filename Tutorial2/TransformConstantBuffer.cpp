#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Renderer& renderer, const Drawable& parent) : parent(parent)
{
	if ( !pVertexConstantBuffer ) pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>( renderer );
}

void TransformConstantBuffer::Bind(Renderer& renderer) noexcept
{
	const auto model = parent.GetTransformXM();

	const Transforms transforms =
	{
		DirectX::XMMatrixTranspose( model ),
		DirectX::XMMatrixTranspose( model * renderer.GetView() * renderer.GetProjection() )
	};

	pVertexConstantBuffer->Update(renderer, transforms );
	pVertexConstantBuffer->Bind(renderer);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVertexConstantBuffer;