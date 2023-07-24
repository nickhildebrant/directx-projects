#pragma once
#include <vector>
#include <array>
#include "IndexedTriangleList.h"

class Plane {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int xDivisions, int yDivisions)
	{
		assert(xDivisions >= 1);
		assert(yDivisions >= 1);

		constexpr float width = 2.0f;
		constexpr float height = 2.0f;

		const int numberVerticesX = xDivisions + 1;
		const int numberVerticesY = yDivisions + 1;

		std::vector<V> vertices(numberVerticesX * numberVerticesY);

		{
			const float sideX = width / 2.0f;
			const float sideY = height / 2.0f;

			const float xDivisionSize = width / float(xDivisions);
			const float yDivisionSize = height / float(yDivisions);

			const auto bottomLeft = DirectX::XMVectorSet(-sideX, -sideY, 0.0f, 0.0f);

			for (int y = 0, i = 0; y < numberVerticesY; y++)
			{
				const float yPosition = float(y) * yDivisionSize;
				for (int x = 0; x < numberVerticesX; x++, i++)
				{
					const auto vec = DirectX::XMVectorAdd(bottomLeft, DirectX::XMVectorSet(float(x) * xDivisionSize, yPosition, 0.0f, 0.0f));
					DirectX::XMStoreFloat3(&vertices[i].position , vec);
				}
			}
		}

		std::vector<unsigned short> indices;
		float divisions = xDivisions * yDivisions;
		indices.reserve((divisions * divisions) * 6);
		{
			const auto vertices2i = [numberVerticesX](size_t x, size_t y)
			{
				return (unsigned short)(y * numberVerticesX + x);
			};

			for (size_t y = 0; y < yDivisions; y++)
			{
				for (size_t x = 0; x < xDivisions; x++)
				{
					const std::array<unsigned short, 4> indexArray = 
					{ 
						vertices2i(x, y), vertices2i(x + 1, y), 
						vertices2i(x, y + 1), vertices2i(x + 1, y + 1) 
					};

					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(1, 1);
	}
};