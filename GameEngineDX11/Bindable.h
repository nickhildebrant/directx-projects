#pragma once
#include "Renderer.h"

class Bindable {
public:
	virtual void Bind( Renderer& renderer );
	virtual ~Bindable() = default;
};