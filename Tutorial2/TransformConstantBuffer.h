#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable {
public:
	TransformConstantBuffer(Renderer& renderer, const Drawable& parent, UINT slot = 0);
	void Bind(Renderer& renderer) noexcept override;

protected:
	struct Transforms {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldViewProjection;
	};

	void UpdateBindImpl( Renderer& renderer, const Transforms& tf );
	Transforms GetTransforms( Renderer& renderer );

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;
	const Drawable& parent;
};