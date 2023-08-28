#pragma once
#include "DrawableBase.h"

class SolidSphere : public DrawableBase<SolidSphere> {
public:
	SolidSphere( Renderer& renderer, float radius );

	void SetPosition( DirectX::XMFLOAT4 pos ) noexcept;

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT4 position = { 1.0f,1.0f,1.0f,1.0f };
};