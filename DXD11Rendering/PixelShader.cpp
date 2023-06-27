#include "PixelShader.h"
#include "RendererErrorMacros.h"

PixelShader::PixelShader(Renderer& renderer, const std::wstring& path)
{
	INFO_MANAGER(renderer);

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(GetDevice(renderer)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShader));
}

void PixelShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->PSSetShader(m_pixelShader.Get(), nullptr, 0u);
}