#pragma once
#include "DrawableBase.h"
#include <random>

class Ball : public DrawableBase<Ball> {
public:
	Ball( Renderer& renderer, float rad, int latitudeSize, int longitudeSize );

	void Update( float dt ) override;
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	float radius;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	DirectX::XMFLOAT3X3 modelTransform;
};
