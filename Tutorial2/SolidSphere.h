#pragma once
#include "Drawable.h"

class SolidSphere : public Drawable {
public:
	SolidSphere( Renderer& renderer, float radius );

	void SetPosition( DirectX::XMFLOAT4 pos ) noexcept;

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT4 position = { 1.0f,1.0f,1.0f,1.0f };
};