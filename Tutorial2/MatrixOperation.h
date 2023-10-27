#pragma once
#include <DirectXMath.h>

DirectX::XMFLOAT4 ExtractEulerAngles(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT4 ExtractTranslation(const DirectX::XMFLOAT4X4& matrix);