#include "Box.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Cube.h"

Box::Box( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT4 materialColor ) 
	:
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 normal;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

		auto pvs = std::make_unique<VertexShader>(renderer, L"PhongVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"PhongPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(renderer, inputDesc, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(renderer, *this));

	struct MaterialConstant {
		DirectX::XMFLOAT4 color;
		float specularIntensity = 1.0f;
		float shininess = 20.f;
		float padding[2];
	} colorConstant;

	colorConstant.color = materialColor;

	AddBind( std::make_unique<PixelConstantBuffer<MaterialConstant>>( renderer, colorConstant, 1u ) );

	// model deformation - per instance
	DirectX::XMStoreFloat4x4(&modelTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4( &modelTransform ) * TestPoly::GetTransformXM();
}