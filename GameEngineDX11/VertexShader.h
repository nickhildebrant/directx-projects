#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader( Renderer& renderer, const std::wstring& path );

	void Bind( Renderer& renderer ) override;
	ID3DBlob* GetBytecode() const;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};
