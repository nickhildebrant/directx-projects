#pragma once
#include "Bindable.h"

class Topology : public Bindable {
public:
	Topology(Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Renderer& renderer) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type );
	static std::string GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type );
	std::string GetUID() const override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
