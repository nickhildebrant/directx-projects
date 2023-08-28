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

	renderer.DrawIndexed( pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind) noexcept
{
	// handle index buffer special case
	if ( typeid( *bind ) == typeid( IndexBuffer ) )
	{
		assert( "Binding multiple index buffers not allowed" && pIndexBuffer == nullptr );
		pIndexBuffer = &static_cast<IndexBuffer&>( *bind );
	}

	binds.push_back(std::move(bind));
}