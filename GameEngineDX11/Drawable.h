#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class Bindable;

class Drawable {
	template<class T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const;

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