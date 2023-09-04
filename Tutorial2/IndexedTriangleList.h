#pragma once
#include <vector>
#include <DirectXMath.h>
#include "VertexLayout.h"

class IndexedTriangleList {
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( VertexHandler::VertexBuffer vertices_in, std::vector<unsigned short> indices_in ) : vertices(std::move(vertices_in)), indices(std::move(indices_in))
	{
		assert(vertices.Size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		using Elements = VertexHandler::VertexLayout::ElementType;
		for (int i = 0; i < vertices.Size(); i++)
		{
			DirectX::XMFLOAT4& position = vertices[i].Attr<Elements::Position3D>();
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat4(&position);
			DirectX::XMStoreFloat4(&position, DirectX::XMVector4Transform(pos, matrix));
		}
	}

	void SetNormalsIndependentFlat() noexcept
	{
		using Type = VertexHandler::VertexLayout::ElementType;

		assert( indices.size() % 3 == 0 && indices.size() > 0 );

		for ( size_t i = 0; i < indices.size(); i += 3 )
		{
			VertexHandler::Vertex v0 = vertices[indices[i]];
			VertexHandler::Vertex v1 = vertices[indices[i + 1]];
			VertexHandler::Vertex v2 = vertices[indices[i + 2]];

			const DirectX::XMVECTOR p0 = DirectX::XMLoadFloat4( &v0.Attr<Type::Position3D>() );
			const DirectX::XMVECTOR p1 = DirectX::XMLoadFloat4( &v1.Attr<Type::Position3D>() );
			const DirectX::XMVECTOR p2 = DirectX::XMLoadFloat4( &v2.Attr<Type::Position3D>() );

			const DirectX::XMVECTOR cross = DirectX::XMVector3Cross( DirectX::XMVectorSubtract( p1, p0 ), DirectX::XMVectorSubtract( p2, p0 ) );
			const DirectX::XMVECTOR normal = DirectX::XMVector4Normalize( cross );

			DirectX::XMStoreFloat4( &v0.Attr<Type::Normal>(), normal );
			DirectX::XMStoreFloat4( &v1.Attr<Type::Normal>(), normal );
			DirectX::XMStoreFloat4( &v2.Attr<Type::Normal>(), normal );
		}
	}

public:
	VertexHandler::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};