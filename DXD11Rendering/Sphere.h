#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Sphere {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int latitudeDivision, int longitudeDivision)
	{
		assert(latitudeDivision >= 3);
		assert(longitudeDivision >= 3);

		constexpr float radius = 1.0f;
		const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);

		const float PI = 3.14159265f;
		const float latitudeAngle = PI / latitudeDivision;
		const float longitudeAngle = 2.0f * PI / longitudeDivision;

		std::vector<V> vertices;
		for (int iLatitude = 1; iLatitude < latitudeDivision; iLatitude++)
		{

			const auto latitudeBase = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(latitudeAngle * iLatitude));

			for (int iLongitude = 0; iLongitude < longitudeDivision; iLongitude++)
			{
				vertices.emplace_back();
				auto vec = DirectX::XMVector3Transform(latitudeBase, DirectX::XMMatrixRotationZ(longitudeAngle * iLongitude));
			}
		}

		// top vertices
		const auto iNorthPole = (unsigned short)vertices.size();
		vertices.emplace_back();

		DirectX::XMStoreFloat3(&vertices.back().position, base);

		const auto iSouthPole = (unsigned short)vertices.size();
		vertices.emplace_back();

		DirectX::XMStoreFloat3(&vertices.back().position, DirectX::XMVectorNegate(base));

		const auto calculateIndex = [latitudeDivision, longitudeDivision](unsigned short iLatitude, unsigned short iLongitude) { return iLatitude * longitudeDivision + iLongitude; };

		std::vector<unsigned short> indices;
		for (unsigned short iLatitude = 0; iLatitude < latitudeDivision - 2; iLatitude++)
		{
			for (unsigned short iLongitude = 0; iLongitude < longitudeDivision - 1; iLongitude++)
			{
				indices.push_back(calculateIndex(iLatitude, iLongitude));
				indices.push_back(calculateIndex(iLatitude + 1, iLongitude));
				indices.push_back(calculateIndex(iLatitude, iLongitude + 1));
				indices.push_back(calculateIndex(iLatitude, iLongitude + 1));
				indices.push_back(calculateIndex(iLatitude + 1, iLongitude));
				indices.push_back(calculateIndex(iLatitude + 1, iLongitude + 1));
			}

			// wrap band
			indices.push_back(calculateIndex(iLatitude, longitudeDivision - 1));
			indices.push_back(calculateIndex(iLatitude + 1, longitudeDivision - 1));
			indices.push_back(calculateIndex(iLatitude, 0));
			indices.push_back(calculateIndex(iLatitude, 0));
			indices.push_back(calculateIndex(iLatitude + 1, longitudeDivision - 1));
			indices.push_back(calculateIndex(iLatitude + 1, 0));
		}

		// cap fans
		for (unsigned short iLongitude = 0; iLongitude < longitudeDivision - 1; iLongitude++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calculateIndex(0, iLongitude));
			indices.push_back(calculateIndex(0, iLongitude + 1));

			// south
			indices.push_back(calculateIndex(latitudeDivision - 2, iLongitude + 1));
			indices.push_back(calculateIndex(latitudeDivision - 2, iLongitude));
			indices.push_back(iSouthPole);
		}

		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calculateIndex(0, longitudeDivision - 1));
		indices.push_back(calculateIndex(0, 0));

		// south
		indices.push_back(calculateIndex(latitudeDivision - 2, 0));
		indices.push_back(calculateIndex(latitudeDivision - 2, longitudeDivision - 1));
		indices.push_back(iSouthPole);

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(12, 24);
	}
};