#include <cassert>

#include "Drawable.h"
#include "IndexBuffer.h"

void Drawable::Draw( Renderer& renderer ) const
{
	for ( auto& bind : binds )
	{
		bind->Bind( renderer );
	}

	for ( auto& bind : GetStaticBinds() )
	{
		bind->Bind( renderer );
	}

	renderer.DrawIndexed( pIndexBuffer->GetCount() );
}

DirectX::XMMATRIX Drawable::GetTransformXM() const { return DirectX::XMMATRIX(); }

void Drawable::Update( float dt ) {}

const std::vector<std::unique_ptr<Bindable>>& Drawable::GetStaticBinds() const { return binds; }

void Drawable::AddBind( std::unique_ptr<Bindable> bind )
{
	assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid( *bind ) != typeid( IndexBuffer ) );
	binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBuffer( std::unique_ptr<IndexBuffer> indexBuffer )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = indexBuffer.get();
	binds.push_back( std::move( indexBuffer ) );
}