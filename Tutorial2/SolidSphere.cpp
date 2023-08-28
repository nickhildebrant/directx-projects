#include "SolidSphere.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Sphere.h"


SolidSphere::SolidSphere( Renderer& renderer, float radius )
{
	struct Vertex {
		DirectX::XMFLOAT4 position;
	};

	auto model = Sphere::MakeTesselated<Vertex>(8, 16);
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

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind( std::make_shared<InputLayout>( renderer, ied, pvsbc ) );

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