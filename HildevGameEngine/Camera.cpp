#include "Camera.h"
//#include "ImGUI/imgui.h"

DirectX::XMMATRIX Camera::GetMatrix() const
{
	const auto position = DirectX::XMVector4Transform( DirectX::XMVectorSet( 0.0f, 0.0f, -r, 0.0f ), DirectX::XMMatrixRotationRollPitchYaw( phi, -theta, 0.0f ) );

	return DirectX::XMMatrixLookAtLH( position, DirectX::XMVectorZero(), DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) ) * DirectX::XMMatrixRotationRollPitchYaw( pitch, -yaw, roll );
}

DirectX::XMVECTOR Camera::GetPosition() const
{
	return DirectX::XMVector4Transform( DirectX::XMVectorSet( 0.0f, 0.0f, -r, 0.0f ), DirectX::XMMatrixRotationRollPitchYaw( phi, -theta, 0.0f ) );
}

void Camera::SpawnControlWindow()
{
	/*if ( ImGui::Begin( "Camera" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "R", &r, 0.1f, 80.0f, "%.1f" );
		ImGui::SliderAngle( "Theta", &theta, -180.0f, 180.0f );
		ImGui::SliderAngle( "Phi", &phi, -90.0f, 90.0f );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -180.0f, 180.0f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.0f, 180.0f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.0f, 180.0f );

		if ( ImGui::Button( "Reset" ) )
		{
			ResetView();
		}
	}

	ImGui::End();*/
}

void Camera::ResetView()
{
	// distance and rotation
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;

	// orientation
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}