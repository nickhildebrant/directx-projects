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
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	struct Vertex {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texCoord;
	};

	auto model = Cube::MakeIndependentTextured<Vertex>();
	model.SetNormalsIndependentFlat();

	AddBind( std::make_shared<Texture>( renderer, Surface::FromFile( "Images\\walter.png" ) ) );

	AddBind( std::make_shared<VertexBuffer>( renderer, model.vertices ) );

	AddBind( std::make_shared<Sampler>( renderer ) );

	auto pvs = std::make_shared<VertexShader>( renderer, L"TexturePhongVertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	AddBind( std::move( pvs ) );

	AddBind( std::make_shared<PixelShader>( renderer, L"TexturePhongPixelShader.cso" ) );

	AddBind( std::make_shared<IndexBuffer>( renderer, model.indices ) );

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind( std::make_shared<InputLayout>( renderer, ied, pvsbc ) );

	AddBind( std::make_shared<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	struct PixelShaderConstant {
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} colorConstant;

	AddBind( std::make_shared<PixelConstantBuffer<PixelShaderConstant>>( renderer, colorConstant, 1u ) );

	AddBind( std::make_shared<TransformConstantBuffer>( renderer, *this ) );
}