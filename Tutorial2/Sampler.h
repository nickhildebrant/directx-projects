#pragma once
#include "Bindable.h"

class Sampler : public Bindable {
public:
	Sampler( Renderer& renderer );
	void Bind( Renderer& renderer ) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer );
	static std::string GenerateUID();
	std::string GetUID() const override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};