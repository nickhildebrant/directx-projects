#include "TexturedBox.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"


TexturedBox::TexturedBox( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist ) 
	:
	r( rdist( rng ) ), droll( ddist( rng ) ), dpitch( ddist( rng ) ), dyaw( ddist( rng ) ),
	dphi( odist( rng ) ), dtheta( odist( rng ) ), dchi( odist( rng ) ), 
	chi( adist( rng ) ), theta( adist( rng ) ), phi( adist( rng ) )
{
	if ( !IsStaticInitialized() )
	{
		struct Vertex {
			DirectX::XMFLOAT3 pos;

			struct {
				float u;
				float v;
			} tex;
		};

		const auto model = Cube::MakeSkinned<Vertex>();

		AddStaticBind( std::make_unique<Texture>( renderer, Surface::FromFile( "Images\\cube.png" ) ) );

		AddStaticBind( std::make_unique<VertexBuffer>( renderer, model.vertices ) );

		AddStaticBind( std::make_unique<Sampler>( renderer ) );

		auto pvs = std::make_unique<VertexShader>( renderer, L"TextureVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( renderer, L"TexturePixelShader.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( renderer, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

void TexturedBox::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX TexturedBox::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}