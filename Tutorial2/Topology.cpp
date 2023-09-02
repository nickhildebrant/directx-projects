#include "Topology.h"
#include "BindableCodex.h"

Topology::Topology(Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type) : type(type)
{

}

void Topology::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->IASetPrimitiveTopology(type);
}

std::shared_ptr<Topology> Topology::Resolve( Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type )
{
	return Codex::Resolve<Topology>( renderer, type );
}

std::string Topology::GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type )
{
	using namespace std::string_literals;
	return typeid( Topology ).name() + "#"s + std::to_string( type );
}

std::string Topology::GetUID() const
{
	return GenerateUID( type );
}