#include "DualTransformCBuffer.h"

DualTransformCBuffer::DualTransformCBuffer( Renderer& renderer, const Drawable& parent, UINT slotV, UINT slotP ) 
	: TransformConstantBuffer( renderer, parent, slotV )
{
	if ( !pPixelConstantBuffer )
	{
		pPixelConstantBuffer = std::make_unique<PixelConstantBuffer<Transforms>>( renderer, slotP );
	}
}

void DualTransformCBuffer::Bind( Renderer& renderer ) noexcept
{
	TransformConstantBuffer::Transforms tf = GetTransforms( renderer );
	TransformConstantBuffer::UpdateBindImpl( renderer, tf );
	UpdateBindImpl( renderer, tf );
}

void DualTransformCBuffer::UpdateBindImpl( Renderer& renderer, const Transforms& tf )
{
	pPixelConstantBuffer->Update( renderer, tf );
	pPixelConstantBuffer->Bind( renderer );
}

std::unique_ptr<PixelConstantBuffer<TransformConstantBuffer::Transforms>> DualTransformCBuffer::pPixelConstantBuffer;