#pragma once
#include "DrawableBase.h"

template<class T>
class TestPoly : public DrawableBase<T> {
public:
	TestPoly( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist )
		:
		r( rdist( rng ) ), droll( ddist( rng ) ), dpitch( ddist( rng ) ), dyaw( ddist( rng ) ), dphi( odist( rng ) ),
		dtheta( odist( rng ) ), dchi( odist( rng ) ), chi( adist( rng ) ), theta( adist( rng ) ), phi( adist( rng ) )
	{}

	void Update( float dt ) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
			DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
			DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
	}

private:
	// positional
	float r;
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
};