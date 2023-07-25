#include "SolidSphere.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Sphere.h"


SolidSphere::SolidSphere( Renderer& renderer, float radius )
{
	if ( !IsStaticInitialized() )
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};
		auto model = Sphere::Make<Vertex>();
		model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );
		AddBind( std::make_unique<VertexBuffer>( renderer, model.vertices ) );
		AddIndexBuffer( std::make_unique<IndexBuffer>( renderer, model.indices ) );

		auto pvs = std::make_unique<VertexShader>( renderer, L"SolidVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx, L"SolidPixelShader.cso" ) );

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSColorConstant>>( gfx, colorConst ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx, ied, pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCbuf>( gfx, *this ) );
}

void SolidSphere::Update( float dt ) noexcept {}

void SolidSphere::SetPos( DirectX::XMFLOAT3 pos ) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation( pos.x, pos.y, pos.z );
}