#pragma once
#include "Bindable.h"

class Topology : public Bindable {
public:
	Topology(Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Renderer& renderer) noexcept override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
