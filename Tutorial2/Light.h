#pragma once
#include "Renderer.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class Light {
public:
	Light( Renderer& renderer, float radius = 0.5f );
	
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;

	void Draw( Renderer& renderer ) const noexcept;
	void Bind( Renderer& renderer ) const noexcept;

private:
	struct LightConstantBuffer {
		DirectX::XMFLOAT3 position;
		float padding;
	};

	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };;
	mutable SolidSphere sphereMesh;
	mutable PixelConstantBuffer<LightConstantBuffer> constantBuffer;
};