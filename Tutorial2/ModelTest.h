#pragma once
#include "TestPoly.h"
#include "ConstantBuffers.h"

class ModelTest : public TestPoly<ModelTest> {
public:
	ModelTest( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, DirectX::XMFLOAT4 material, float scale );
};