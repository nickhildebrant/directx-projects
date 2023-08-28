#include "Box.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"
#include "Cube.h"
#include "ImGUI/imgui.h"

Box::Box( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT4 materialColor ) 
	:
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	struct Vertex {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 normal;
	};

	auto model = Cube::MakeIndependent<Vertex>();
	model.SetNormalsIndependentFlat();

	AddBind(std::make_shared<VertexBuffer>(renderer, model.vertices));

	auto pvs = std::make_shared<VertexShader>(renderer, L"PhongVertexShader.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_shared<PixelShader>(renderer, L"PhongPixelShader.cso"));

	AddBind(std::make_shared<IndexBuffer>(renderer, model.indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	AddBind(std::make_shared<InputLayout>(renderer, inputDesc, pvsbc));

	AddBind(std::make_shared<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformConstantBuffer>(renderer, *this));

	m_materialConstants.color = materialColor;

	AddBind( std::make_shared<PixelConstantBuffer<PSMaterialConstant>>( renderer, m_materialConstants, 1u ) );

	// model deformation - per instance
	DirectX::XMStoreFloat4x4(&modelTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

bool Box::SpawnControlWindow( Renderer& renderer, int id ) noexcept
{
	using namespace std::string_literals;

	bool dirty = false, open = true;
	if ( ImGui::Begin( ( "Box "s + std::to_string( id ) ).c_str(), &open ) )
	{
		ImGui::Text( "Material Properties" );
		const auto cd = ImGui::ColorEdit3( "Material Color", &m_materialConstants.color.x );
		const auto sid = ImGui::SliderFloat( "Specular Intensity", &m_materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", 2 );
		const auto spd = ImGui::SliderFloat( "Specular Power", &m_materialConstants.specularPower, 1.0f, 200.0f, "%.2f", 2 );
		dirty = cd || sid || spd;

		ImGui::Text( "Position" );
		ImGui::SliderFloat( "R", &r, 0.0f, 80.0f, "%.1f" );
		ImGui::SliderAngle( "Theta", &theta, -180.0f, 180.0f );
		ImGui::SliderAngle( "Phi", &phi, -180.0f, 180.0f );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -180.0f, 180.0f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.0f, 180.0f );
	}

	ImGui::End();

	if ( dirty ) { SyncMaterial( renderer ); }

	return open;
}

void Box::SyncMaterial( Renderer& renderer ) noexcept
{
	auto pConstPS = SearchBindable<PixelConstantBuffer<PSMaterialConstant>>();
	pConstPS->Update( renderer, m_materialConstants );
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4( &modelTransform ) * TestPoly::GetTransformXM();
}