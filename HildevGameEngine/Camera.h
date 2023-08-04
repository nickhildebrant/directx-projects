#pragma once
#include "Renderer.h"

class Camera {
public:
	DirectX::XMMATRIX GetMatrix() const;
	DirectX::XMVECTOR GetPosition() const;
	void SpawnControlWindow();
	void ResetView();

private:
	// distance and rotation
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;

	// orientation
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};