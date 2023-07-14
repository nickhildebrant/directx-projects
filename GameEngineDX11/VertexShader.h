#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader( Renderer& renderer, const std::wstring& path )
	{
		D3DReadFileToBlob( path.c_str(), &pBlob );
		renderer.GetDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader );
	}

	void Bind( Renderer& renderer ) override
	{
		renderer.GetDeviceContext()->VSSetShader( pVertexShader.Get(), nullptr, 0u );
	}

	ID3DBlob* GetBytecode() const { return pBlob.Get(); }

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};
