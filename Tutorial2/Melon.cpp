#include "Melon.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Sphere.h"

Melon::Melon(Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, 
	std::uniform_int_distribution<int>& longdist, std::uniform_int_distribution<int>& latdist)
	:
	r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)),
	dtheta(odist(rng)), dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng))
{
	if (!IsStaticInitialized())
	{
		auto pvs = std::make_unique<VertexShader>(renderer, L"ColorIndexVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"ColorIndexPixelShader.cso"));

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
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};

		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(renderer, constantBuffer));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(renderer, inputElementDescription, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};

	auto model = Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));

	// deform vertices of model by linear transformation
	model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

	AddBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

	AddIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

	AddBind(std::make_unique<TransformConstantBuffer>(renderer, *this));
}

void Melon::Update(float deltaTime) noexcept
{
	roll += droll * deltaTime;
	pitch += dpitch * deltaTime;
	yaw += dyaw * deltaTime;
	theta += dtheta * deltaTime;
	phi += dphi * deltaTime;
	chi += dchi * deltaTime;
}

DirectX::XMMATRIX Melon::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DirectX::XMMatrixTranslation( r, 0.0f, 0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta, phi, chi );
}