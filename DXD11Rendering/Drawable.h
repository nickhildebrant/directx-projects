#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
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
	const IndexBuffer* m_indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};