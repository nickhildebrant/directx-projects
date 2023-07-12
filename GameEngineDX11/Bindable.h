#pragma once
#include "Renderer.h"

class Bindable {
public:
	virtual void Bind( Renderer& renderer );
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext( Renderer& renderer );
	static ID3D11Device* GetDevice( Renderer& renderer );
};