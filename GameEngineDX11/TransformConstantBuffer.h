#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable {
public:
	TransformConstantBuffer( Renderer& renderer, const Drawable& parent );
	void Bind( Renderer& renderer ) override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> vertexConstantBuffer;
	const Drawable& parent;
};
