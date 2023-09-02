#pragma once
#include "Bindable.h"
#include "VertexLayout.h"

class InputLayout : public Bindable {
public:
	InputLayout( Renderer& renderer, VertexHandler::VertexLayout layout, ID3DBlob* vertexShaderBlob );
	void Bind(Renderer& renderer) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, const VertexHandler::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode );
	static std::string GenerateUID( const VertexHandler::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr );
	std::string GetUID() const noexcept override;

protected:
	VertexHandler::VertexLayout layout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};