#pragma once
#include "TestPoly.h"
#include "ConstantBuffers.h"

class Box : public TestPoly<Box> {
public:
	Box(Renderer& renderer, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT4 materialColor);

	void SpawnControlWindow( Renderer& renderer, int id ) noexcept;

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT4X4 modelTransform;

	struct PSMaterialConstant {
		DirectX::XMFLOAT4 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} m_materialConstants;

	void SyncMaterial( Renderer& renderer ) noexcept;
};