#pragma once
#include "Renderer.h"
#include "InputLayout.h"
#include "Topology.h"

#include "ConstantBuffers.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "TransformConstantBuffer.h"

#include "PixelShader.h"
#include "VertexShader.h"

class Bindable {
public:
	virtual void Bind( Renderer& renderer );
	virtual ~Bindable() = default;
};