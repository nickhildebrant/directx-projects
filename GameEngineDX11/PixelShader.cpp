#include "PixelShader.h"

PixelShader::PixelShader( Renderer& renderer, const std::wstring& path )
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob( path.c_str(), &blob );
	renderer.GetDevice()->CreatePixelShader( blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pPixelShader );
}

void PixelShader::Bind( Renderer& renderer )
{
	renderer.GetDeviceContext()->PSSetShader( pPixelShader.Get(), nullptr, 0u );
}