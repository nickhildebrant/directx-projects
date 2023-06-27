#include "Drawable.h"
#include "RendererErrorMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Renderer& renderer) const noexcept
{
	for (auto& bind : binds)
	{
		bind->Bind(renderer);
	}

	for (auto& bind : GetStaticBinds())
	{
		bind->Bind(renderer);
	}

	renderer.DrawIndexed(m_indexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && m_indexBuffer == nullptr);
	m_indexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}