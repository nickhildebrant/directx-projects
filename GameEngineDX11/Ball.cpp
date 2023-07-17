#include "Bindable.h"
#include "BindableBase.h"

#include "Sphere.h"
#include "Ball.h"

Ball::Ball( Renderer& renderer, float radius, int latitudeSize, int longitudeSize ) : radius(radius)
{
	if ( !IsStaticInitialized() )
	{
		struct VertexShaderInput {
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 normal;
		};

		struct Vertex {
			DirectX::XMFLOAT3 position;
		};

		auto model = Sphere::Make<Vertex>();
		model.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

		AddStaticBind( std::make_unique<VertexBuffer>( renderer, model.vertices ) );

		auto pVertexShader = std::make_unique<VertexShader>( renderer, L"PhongVertexShader.cso" );
		auto pvsbc = pVertexShader->GetBytecode();
		AddStaticBind( std::move( pVertexShader ) );

		AddStaticBind( std::make_unique<PixelShader>( renderer, L"PhongPixelShader.cso" ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( renderer, model.indices ) );

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};

		const PixelShaderConstants constantBuffer =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};

		AddStaticBind( std::make_unique<PixelConstantBuffer<PixelShaderConstants>>( renderer, constantBuffer ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind( std::make_unique<InputLayout>( renderer, inputDesc, pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformConstantBuffer>( renderer, *this ) );

	// model deformation - per instance
	DirectX::XMStoreFloat3x3( &modelTransform, DirectX::XMMatrixScaling( 1.0f, 1.0f, bdist( rng ) ) );
}

void Ball::Update( float dt )
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Ball::GetTransformXM() const
{
	return DirectX::XMLoadFloat3x3( &modelTransform ) *
		DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DirectX::XMMatrixTranslation( radius, 0.0f, 0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi ) *
		DirectX::XMMatrixTranslation( 0.0f, 0.0f, 20.0f );
}
