#pragma once
#include "Drawable.h"

class TestPlane : public Drawable {
public:
	TestPlane( Renderer& renderer, float size );

	void SetPosition( DirectX::XMFLOAT4 position );
	void SetRotation( float roll, float pitch, float yaw );

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	void SpawnControlWindow( Renderer& renderer );

private:
	DirectX::XMFLOAT4 position = { 1.0f,1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding;
	} materialConstant;
};