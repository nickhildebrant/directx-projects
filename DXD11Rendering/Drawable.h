#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class Bindable;

class Drawable {
	template<class T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	void Draw(Renderer& renderer) const noexcept;
	virtual void Update(float dt) noexcept = 0;

	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

	const IndexBuffer* m_indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};