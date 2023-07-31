#include "Pyramid.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Cone.h"

Pyramid::Pyramid(Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_int_distribution<int>& tdist )
	:
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 normal;
			DirectX::XMFLOAT4 color;
		};

		int tesselation = tdist( rng );
		auto model = Cone::MakeTesselatedIndependentFaces<Vertex>( tesselation );

		// set vertex colors for mesh
		model.vertices[0].color = { 1.0f,	1.0f,	0.0f,	1.0f };
		model.vertices[1].color = { 1.0f,	1.0f,	0.0f,	1.0f };
		model.vertices[2].color = { 1.0f,	1.0f,	0.0f,	1.0f };
		model.vertices[3].color = { 1.0f,	1.0f,	0.0f,	1.0f };
		model.vertices[4].color = { 1.0f,	1.0f,	0.8f,	1.0f };
		model.vertices[5].color = { 1.0f,	0.1f,	0.0f,	1.0f };

		// deform mesh linearly
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 0.7f));

		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

		auto pvs = std::make_unique<VertexShader>(renderer, L"BlendedPhongVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"BlendedPhongPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(renderer, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		
		struct PixelShaderConstant {
			float specularIntensity = 0.6f;
			float shininess = 30.0f;
			float padding[2];
		} colorConstant;

		AddStaticBind( std::make_unique<PixelConstantBuffer<PixelShaderConstant>>( renderer, colorConstant, 1u ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(renderer, *this));
}