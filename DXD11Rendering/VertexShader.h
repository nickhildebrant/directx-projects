#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Renderer& renderer, const std::wstring& path);
	void Bind(Renderer& renderer) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};