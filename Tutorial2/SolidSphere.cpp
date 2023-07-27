#include "SolidSphere.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Sphere.h"


SolidSphere::SolidSphere( Renderer& renderer, float radius )
{
	if ( !IsStaticInitialized() )
	{
		struct Vertex {
			DirectX::XMFLOAT4 position;
		};

		auto model = Sphere::Make<Vertex>();
		model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

		AddBind( std::make_unique<VertexBuffer>( renderer, model.vertices ) );

		AddIndexBuffer( std::make_unique<IndexBuffer>( renderer, model.indices ) );

		auto pvs = std::make_unique<VertexShader>( renderer, L"SolidVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( renderer, L"SolidPixelShader.cso" ) );

		struct PSColorConstant {
			DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		} colorConst;

		AddStaticBind( std::make_unique<PixelConstantBuffer<PSColorConstant>>( renderer, colorConst ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind( std::make_unique<InputLayout>( renderer, ied, pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformConstantBuffer>( renderer, *this ) );
}

void SolidSphere::Update( float dt ) noexcept {}

void SolidSphere::SetPosition( DirectX::XMFLOAT4 pos ) noexcept
{
	this->position = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation( position.x, position.y, position.z );
}