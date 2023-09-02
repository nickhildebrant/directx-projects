#pragma once
#include "Bindable.h"
#include "RendererErrorMacros.h"
#include "VertexLayout.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer( Renderer& renderer, const VertexHandler::VertexBuffer& vertexBuffer );
	void Bind(Renderer& renderer) noexcept override;

protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};