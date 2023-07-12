#pragma once
#include "Renderer.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable {
	template<class T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable( const Drawable& );
	virtual ~Drawable();

	virtual DirectX::XMMATRIX GetTransformXM();

	void Draw( Renderer& renderer ) const;
	virtual void Update( float dt );

protected:
	void AddBind( std::unique_ptr<Bindable> bind );
	void AddIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf );

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const;

	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};