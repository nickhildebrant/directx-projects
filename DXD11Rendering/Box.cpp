#include "Box.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Sphere.h"

Box::Box(Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist) :
	r(rdist(rng)), droll(ddist(rng)), dpitch(ddist(rng)), dyaw(ddist(rng)), dphi(odist(rng)), dtheta(odist(rng)),
	dchi(odist(rng)), chi(adist(rng)), theta(adist(rng)), phi(adist(rng))
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
		};

		auto model = Sphere::Make<Vertex>();
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

		auto pvs = std::make_unique<VertexShader>(renderer, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"PixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};

		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};

		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(renderer, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(renderer, inputDesc, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(renderer, *this));
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}