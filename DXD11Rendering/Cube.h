#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Cube {
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<DirectX::XMFLOAT3> vertices;
		vertices.emplace_back(-side, -side, -side);
		vertices.emplace_back(side, -side, -side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(side, side, -side);
		vertices.emplace_back(-side, -side, side);
		vertices.emplace_back(side, -side, side);
		vertices.emplace_back(-side, side, side);
		vertices.emplace_back(side, side, side);

		std::vector<V> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			verts[i].position = vertices[i];
		}

		return
		{
			std::move(verts),
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}
};