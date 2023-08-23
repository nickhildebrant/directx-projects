#include "Camera.h"
#include "ImGUI/imgui.h"

const float PI = 3.14159265f;

Camera::Camera()
{
	ResetView();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto position = DirectX::XMVector4Transform( DirectX::XMVectorSet( 0.0f, 0.0f, -r, 0.0f ), DirectX::XMMatrixRotationRollPitchYaw( phi, -theta, 0.0f ) );

	return DirectX::XMMatrixLookAtLH( position, DirectX::XMVectorZero(), DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) ) * DirectX::XMMatrixRotationRollPitchYaw( pitch, -yaw, roll );
}

void Camera::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Camera" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X", &position.x, -80.0f, 80.0f, "%.1f" );
		ImGui::SliderFloat( "Y", &position.y, -80.0f, 80.0f, "%.1f" );
		ImGui::SliderFloat( "Z", &position.z, -80.0f, 80.0f, "%.1f" );

		ImGui::Text( "Orientation" );
		ImGui::SliderAngle( "Roll", &roll, -90.0f, 90.0f );
		ImGui::SliderAngle( "Pitch", &pitch, -90.0f, 90.0f );
		ImGui::SliderAngle( "Yaw", &yaw, -180.0f, 180.0f );

		if ( ImGui::Button( "Reset" ) )
		{
			ResetView();
		}
	}

	ImGui::End();
}

void Camera::ResetView() noexcept
{
	position = { 0.0f, 7.5f, -18.0f, 1.0f };

	roll = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
}

void Camera::Translate( DirectX::XMFLOAT4 translation )
{
	DirectX::XMStoreFloat4( &translation, DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &translation ), 
			DirectX::XMMatrixRotationYawPitchRoll( roll, pitch, yaw ) * 
			DirectX::XMMatrixScaling( translationSpeed, translationSpeed, translationSpeed ) ) );

	position = 
	{
		position.x + translation.x,
		position.y + translation.y,
		position.z + translation.z,
		1.0f
	};
}

void Camera::Rotate( float dx, float dy, float dz )
{
	yaw = wrap_angle( yaw + dx * rotationSpeed );
	pitch = std::clamp( pitch + dy * rotationSpeed, -PI / 2.0f, PI / 2.0f );
}

float wrap_angle( float theta )
{
	const float PI = 3.1415926535897932f;
	const float modded = fmod( theta, 2.0f * PI );
	return modded > PI ? ( modded - 2.0f * PI ) : modded;
}