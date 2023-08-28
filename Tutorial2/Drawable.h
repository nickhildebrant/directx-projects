#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Renderer.h"

#include "IndexBuffer.h"

class Bindable;

class Drawable {

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	void Draw(Renderer& renderer) const noexcept;

protected:
	void AddBind(std::shared_ptr<Bindable> bind) noexcept;

	// Can search for a specific bindable to manipulate it
	template<class T>
	T* SearchBindable() noexcept
	{
		for ( auto& pBindable : binds )
		{
			if ( auto pType = dynamic_cast<T*>( pBindable.get() ) ) { return pType; }
		}

		return nullptr;
	}

private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> binds;
};