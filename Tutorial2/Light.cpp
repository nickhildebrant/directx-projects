#include "Light.h"
#include "ImGUI/imgui.h"

Light::Light( Renderer& renderer, float radius ) : sphereMesh(renderer, radius), constantBuffer(renderer)
{
	Reset();
}

void Light::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );

		ImGui::SliderFloat( "X", &bufferData.position.x, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &bufferData.position.y, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &bufferData.position.z, -60.0f, 60.0f, "%.1f" );

		ImGui::Text( "Intensity/Color" );
		ImGui::SliderFloat( "Intensity", &bufferData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2 );
		ImGui::ColorEdit3( "Diffuse Color", &bufferData.diffuseColor.x );
		ImGui::ColorEdit3( "Ambient", &bufferData.ambientColor.x );
		ImGui::ColorEdit3( "Material", &bufferData.materialColor.x );

		ImGui::Text( "Falloff" );
		ImGui::SliderFloat( "Constant", &bufferData.attenuationConstant, 0.05f, 10.0f, "%.2f", 4 );
		ImGui::SliderFloat( "Linear", &bufferData.attenuationLinear, 0.0001f, 4.0f, "%.4f", 8 );
		ImGui::SliderFloat( "Quadratic", &bufferData.attenuationQuadradic, 0.0000001f, 10.0f, "%.7f", 10 );

		if ( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}

	ImGui::End();
}

void Light::Reset() noexcept
{
	bufferData =
	{
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.7f, 0.7f, 0.9f, 1.0f },
		{ 0.05f, 0.05f, 0.05f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void Light::Draw( Renderer& renderer ) const noexcept
{
	sphereMesh.SetPosition( bufferData.position );
	sphereMesh.Draw( renderer );
}

void Light::Bind( Renderer& renderer ) const noexcept
{
	constantBuffer.Update( renderer, LightConstantBuffer{ bufferData } );
	constantBuffer.Bind( renderer );
}