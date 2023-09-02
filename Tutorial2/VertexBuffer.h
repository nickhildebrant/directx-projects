#pragma once
#include "Bindable.h"
#include "RendererErrorMacros.h"
#include "VertexLayout.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer( Renderer& renderer, const std::string& tag, const VertexHandler::VertexBuffer& vertexBuffer );
	VertexBuffer( Renderer& renderer, const VertexHandler::VertexBuffer& vertexBuffer );
	void Bind(Renderer& renderer) noexcept override;

	static std::shared_ptr<Bindable> Resolve( Renderer& renderer, const std::string& tag, const VertexHandler::VertexBuffer& vertexBuffer );

	template<typename...Ignore>
	static std::string GenerateUID( const std::string& tag, Ignore&&...ignore )
	{
		return GenerateUID_( tag );
	}

	std::string GetUID() const override;

protected:
	std::string tag;
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

private:
	static std::string GenerateUID_( const std::string& tag );
};