#include "Light.h"
#include "ImGUI/imgui.h"

Light::Light( Renderer& renderer, float radius ) : sphereMesh(renderer, radius), constantBuffer(renderer)
{

}

void Light::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Light" ) )
	{
		ImGui::Text( "Position" );

		ImGui::SliderFloat( "X", &position.x, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &position.y, -60.0f, 60.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &position.z, -60.0f, 60.0f, "%.1f" );

		if ( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}

	ImGui::End();
}

void Light::Reset() noexcept
{
	position = { 0.0f,0.0f,0.0f };
}

void Light::Bind( Renderer& renderer ) const noexcept
{
	constantBuffer.Bind( renderer );
}