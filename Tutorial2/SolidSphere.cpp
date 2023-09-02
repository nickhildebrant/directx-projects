#include "SolidSphere.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "VertexLayout.h"
#include "Sphere.h"


SolidSphere::SolidSphere( Renderer& renderer, float radius )
{
	auto model = Sphere::Make();
	model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

	AddBind( std::make_shared<VertexBuffer>( renderer, model.vertices ) );

	AddBind( std::make_shared<IndexBuffer>( renderer, model.indices ) );

	auto pvs = std::make_shared<VertexShader>( renderer, L"SolidVertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	AddBind( std::move( pvs ) );

	AddBind( std::make_shared<PixelShader>( renderer, L"SolidPixelShader.cso" ) );

	struct PSColorConstant {
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	} colorConst;

	AddBind( std::make_shared<PixelConstantBuffer<PSColorConstant>>( renderer, colorConst ) );

	AddBind( std::make_shared<InputLayout>( renderer, model.vertices.GetLayout().GetD3DLayout(), pvsbc ) );

	AddBind( std::make_shared<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

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