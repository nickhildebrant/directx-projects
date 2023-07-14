#include "PixelShader.h"

PixelShader::PixelShader( Renderer& renderer, const std::wstring& path )
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob( path.c_str(), &blob );
	renderer.GetDevice()->CreatePixelShader( blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShader );
}

void PixelShader::Bind( Renderer& renderer )
{
	renderer.GetDeviceContext()->PSSetShader( m_pixelShader.Get(), nullptr, 0u );
}