#include "TestPlane.h"
#include "Plane.h"
#include "BindableBase.h"
#include "DualTransformCBuffer.h"
#include "ImGUI/imgui.h"

TestPlane::TestPlane( Renderer& renderer, float size )
{
	auto model = Plane::Make();
	model.Transform( DirectX::XMMatrixScaling( size, size, 1.0f ) );
	const auto geometryTag = "$plane." + std::to_string( size );

	AddBind( VertexBuffer::Resolve( renderer, geometryTag, model.vertices ) );

	AddBind( IndexBuffer::Resolve( renderer, geometryTag, model.indices ) );

	AddBind( Texture::Resolve( renderer, "Images\\brickwall.jpg" ) );
	AddBind( Texture::Resolve( renderer, "Images\\brickwall_normal.jpg", 1u ) );

	auto pvs = VertexShader::Resolve( renderer, "PhongVertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	AddBind( std::move( pvs ) );

	AddBind( PixelShader::Resolve( renderer, "NormalPhongObjectPS.cso" ) );

	AddBind( PixelConstantBuffer<PSMaterialConstant>::Resolve( renderer, materialConstant, 1u ) );

	AddBind( InputLayout::Resolve( renderer, model.vertices.GetLayout(), pvsbc ) );

	AddBind( Topology::Resolve( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_shared<DualTransformCBuffer>( renderer, *this, 0u, 2u ) );
}

void TestPlane::SetPosition( DirectX::XMFLOAT4 position )
{
	this->position = position;
}

void TestPlane::SetRotation( float roll, float pitch, float yaw )
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX TestPlane::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( roll, pitch, yaw ) * DirectX::XMMatrixTranslation( position.x, position.y, position.z );
}

void TestPlane::SpawnControlWindow( Renderer& renderer )
{
	if ( ImGui::Begin( "Plane" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &position.x, -80.0f, 80.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &position.y, -80.0f, 80.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &position.z, -80.0f, 80.0f, "%.1f" );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -180.0f, 180.0f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.0f, 180.0f );

		ImGui::Text( "Shading" );

		bool changed0 = ImGui::SliderFloat( "Spec. Int.", &materialConstant.specularIntensity, 0.0f, 1.0f );
		bool changed1 = ImGui::SliderFloat( "Spec. Power", &materialConstant.shininess, 0.0f, 100.0f );
		bool checkState = materialConstant.normalMappingEnabled == TRUE;
		bool changed2 = ImGui::Checkbox( "Enable Normal Map", &checkState );

		materialConstant.normalMappingEnabled = checkState ? TRUE : FALSE;

		if ( changed0 || changed1 || changed2 )
		{
			SearchBindable<PixelConstantBuffer<PSMaterialConstant>>()->Update( renderer, materialConstant );
		}
	}

	ImGui::End();
}