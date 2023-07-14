#pragma once
#include "Bindable.h"

class Topology : public Bindable {
public:
	Topology( Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type ) : type( type ) {}

	void Bind( Renderer& renderer ) override
	{
		renderer.GetDeviceContext()->IASetPrimitiveTopology( type );
	}

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};

