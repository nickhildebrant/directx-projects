#include "Camera.h"
#include "ImGUI/imgui.h"

const float PI = 3.14159265f;

Camera::Camera()
{
	ResetView();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const DirectX::XMVECTOR forward = DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
	const DirectX::XMVECTOR lookVector = DirectX::XMVector4Transform( forward, DirectX::XMMatrixRotationRollPitchYaw( roll, pitch, yaw ) );

	const DirectX::XMVECTOR cameraPosition = DirectX::XMLoadFloat4( &position );
	const DirectX::XMVECTOR cameraTarget = DirectX::XMVectorAdd( cameraPosition, lookVector );

	return DirectX::XMMatrixLookAtRH( cameraTarget, cameraPosition, DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) );
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
		ImGui::SliderAngle( "Roll", &roll, 0.99f * -90.0f, 0.99f * 90.0f );
		ImGui::SliderAngle( "Pitch", &pitch, -180.0f, 180.0f );
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
			DirectX::XMMatrixRotationRollPitchYaw( roll, pitch, yaw ) * 
			DirectX::XMMatrixScaling( translationSpeed, translationSpeed, translationSpeed ) ) );

	position = 
	{
		position.x + translation.x,
		position.y + translation.y,
		position.z + translation.z,
		1.0f
	};
}

float wrap_angle( float theta )
{
	const float PI = 3.1415926535897932f;
	const float modded = fmod( theta, 2.0f * PI );
	return modded > PI ? ( modded - 2.0f * PI ) : modded;
}

void Camera::Rotate( float dx, float dy, float dz )
{
	roll = std::clamp( roll + dx * rotationSpeed, 0.99f * -PI / 2.0f, 0.99f * PI / 2.0f );
	pitch = wrap_angle( pitch + dy * rotationSpeed );
	yaw = wrap_angle( yaw + dz * rotationSpeed );
}