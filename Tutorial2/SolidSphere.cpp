#include "SolidSphere.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "VertexLayout.h"
#include "Sphere.h"


SolidSphere::SolidSphere( Renderer& renderer, float radius )
{
	auto model = Sphere::Make();
	model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

	const std::string geometryTag = "$sphere." + std::to_string( radius );

	AddBind( VertexBuffer::Resolve( renderer, geometryTag, model.vertices ) );

	AddBind( IndexBuffer::Resolve( renderer, geometryTag, model.indices ) );

	auto pvs = VertexShader::Resolve( renderer, "SolidVertexShader.cso" );
	auto pvsbc = static_cast<VertexShader&>( *pvs ).GetBytecode();
	AddBind( std::move( pvs ) );

	AddBind( PixelShader::Resolve( renderer, "SolidPixelShader.cso" ) );

	struct PSColorConstant {
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	} colorConst;

	AddBind( PixelConstantBuffer<PSColorConstant>::Resolve( renderer, colorConst ) );

	AddBind( InputLayout::Resolve( renderer, model.vertices.GetLayout(), pvsbc ) );

	AddBind( Topology::Resolve( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<TransformConstantBuffer>( renderer, *this ) );
}

void SolidSphere::SetPosition( DirectX::XMFLOAT4 pos ) noexcept
{
	this->position = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation( position.x, position.y, position.z );
}