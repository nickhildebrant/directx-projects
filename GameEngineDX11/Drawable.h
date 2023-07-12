#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class Bindable;

class Drawable {
	template<class T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable( const Drawable& );
	virtual ~Drawable();

	virtual DirectX::XMMATRIX GetTransformXM();

	void Draw( Renderer& renderer );
	virtual void Update( float dt );
};