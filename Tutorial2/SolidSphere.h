#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere> {
public:
	SolidSphere( Renderer& renderer, float radius );

	void Update( float dt ) noexcept override;
	void SetPosition( DirectX::XMFLOAT3 pos ) noexcept;

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT3 position = { 1.0f,1.0f,1.0f };
};