#pragma once
#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout(Renderer& renderer, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescription, ID3DBlob* vertexShaderBlob);
	void Bind(Renderer& renderer) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};