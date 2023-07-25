#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable {
public:
	TransformConstantBuffer(Renderer& renderer, const Drawable& parent);
	void Bind(Renderer& renderer) noexcept override;

private:
	struct Transforms {
		DirectX::XMMATRIX worldViewProjection;
		DirectX::XMMATRIX world;
	};

	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;
	const Drawable& parent;
};