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
		DirectX::XMFLOAT4 position;

		DirectX::XMFLOAT4 materialColor;
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;

		float diffuseIntensity;
		float attenuationConstant;
		float attenuationLinear;
		float attenuationQuadradic;
	};

	LightConstantBuffer bufferData;
	mutable SolidSphere sphereMesh;
	mutable PixelConstantBuffer<LightConstantBuffer> constantBuffer;
};