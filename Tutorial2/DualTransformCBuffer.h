#pragma once
#include "TransformConstantBuffer.h"

class DualTransformCBuffer : public TransformConstantBuffer {
public:
	DualTransformCBuffer( Renderer& renderer, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u );
	void Bind( Renderer& renderer ) noexcept override;

protected:
	void UpdateBindImpl( Renderer& renderer, const Transforms& tf );

private:
	static std::unique_ptr<PixelConstantBuffer<Transforms>> pPixelConstantBuffer;
};