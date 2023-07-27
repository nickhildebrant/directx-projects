#include "Sheet.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Plane.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"


Sheet::Sheet( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
				std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist )
		: r( rdist( rng ) ), droll( ddist( rng ) ), dpitch( ddist( rng ) ), dyaw( ddist( rng ) ), dphi( odist( rng ) ),
		dtheta( odist( rng ) ), dchi( odist( rng ) ), chi( adist( rng ) ), theta( adist( rng ) ), phi( adist( rng ) )
{

	if ( !IsStaticInitialized() )
	{
		struct Vertex {
			DirectX::XMFLOAT4 position;

			struct {
				float u;
				float v;
			} texture;
		};

		auto model = Plane::Make<Vertex>();
		model.vertices[0].texture = { 0.0f, 0.0f };
		model.vertices[1].texture = { 1.0f, 0.0f };
		model.vertices[2].texture = { 0.0f, 1.0f };
		model.vertices[3].texture = { 1.0f, 1.0f };

		AddStaticBind( std::make_unique<Texture>( renderer, Surface::FromFile( "Images\\kappa50.png" ) ) );

		AddStaticBind( std::make_unique<VertexBuffer>( renderer, model.vertices ) );

		AddStaticBind( std::make_unique<Sampler>( renderer ) );

		auto pvs = std::make_unique<VertexShader>( renderer, L"TextureVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();

		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( renderer, L"TexturePixelShader.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( renderer, model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

void Sheet::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}