#pragma once
#include "Renderer.h"

class Camera {
public:
	Camera();
	
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void ResetView() noexcept;

	void Translate( DirectX::XMFLOAT4 translation );
	void Rotate( float dx, float dy, float dz );

private:
	DirectX::XMFLOAT4 position;
	float roll;
	float pitch;
	float yaw;

	static constexpr float translationSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};