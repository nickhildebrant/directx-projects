#include "Cylinder.h"
#include "Prism.h"
#include "BindableBase.h"

Cylinder::Cylinder( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist, std::uniform_int_distribution<int>& tdist )
	:
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	auto pvs = std::make_shared<VertexShader>( renderer, L"PhongVertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	AddBind( std::move( pvs ) );

	AddBind( std::make_shared<PixelShader>( renderer, L"IndexedPhongPixelShader.cso" ) );

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind( std::make_shared<InputLayout>( renderer, ied, pvsbc ) );

	AddBind( std::make_shared<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	struct PSMaterialConstant
	{
		alignas(16) DirectX::XMFLOAT4 colors[6] = {
			{1.0f,0.0f,0.0f,1.0f},
			{0.0f,1.0f,0.0f,1.0f},
			{0.0f,0.0f,1.0f,1.0f},
			{1.0f,1.0f,0.0f,1.0f},
			{1.0f,0.0f,1.0f,1.0f},
			{0.0f,1.0f,1.0f,1.0f},
		};

		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
	} materialConstant;

	AddBind( std::make_shared<PixelConstantBuffer<PSMaterialConstant>>( renderer, materialConstant, 1u ) );
	
	struct Vertex {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
	};

	auto model = Prism::MakeTesselatedIndependentCapNormals<Vertex>( tdist( rng ) );

	AddBind( std::make_shared<VertexBuffer>( renderer, model.vertices ) );

	AddBind( std::make_shared<IndexBuffer>( renderer, model.indices ) );

	AddBind( std::make_shared<TransformConstantBuffer>( renderer, *this ) );
}