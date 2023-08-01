#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

const float PI = 3.14159265f;

class Cone {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longitudeDivisions)
	{
		assert(longitudeDivisions >= 3);

		const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longitudeDivisions;

		// base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			vertices.emplace_back();
			auto vec = DirectX::XMVector4Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iLong));
			DirectX::XMStoreFloat4(&vertices.back().position, vec);
		}

		// the center
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,-1.0f,1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);

		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().position = { 0.0f,0.0f,1.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longitudeDivisions);
			indices.push_back(iLong);
		}

		// cone indices
		for (unsigned short iLong = 0; iLong < longitudeDivisions; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % longitudeDivisions);
			indices.push_back(iTip);
		}

		return { std::move(vertices), std::move(indices) };
	}

    template<class V>
    static IndexedTriangleList<V> MakeTesselatedIndependentFaces( int longDiv )
    {
        assert( longDiv >= 3 );

        constexpr float radius = 1.0f;
        const float longitudeAngle = 2.0f * PI / longDiv;

        std::vector<V> vertices;
        std::vector<unsigned short> indices;

        // Cone vertices
        const unsigned short iConeTip = (unsigned short) vertices.size();
        vertices.emplace_back(); // Tip vertex at (0, 0, 1)
        vertices.back().position = { 0.0f, 0.0f, radius, 1.0f };

        for ( int iLong = 0; iLong < longDiv; iLong++ )
        {
            const float theta0 = longitudeAngle * iLong;
            const float theta1 = longitudeAngle * ( iLong + 1 );

            V vertex0, vertex1;
            vertex0.position = { radius * std::cos( theta0 ), radius * std::sin( theta0 ), -radius, 1.0f };
            vertex1.position = { radius * std::cos( theta1 ), radius * std::sin( theta1 ), -radius, 1.0f };

            vertices.push_back( vertex0 );
            vertices.push_back( vertex1 );
        }

        // Cone indices
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iConeTip );
            indices.push_back( iLong * 2 + 1 );
            indices.push_back( ( iLong + 1 ) % longDiv * 2 + 1 );
        }

        // Base vertices
        const unsigned short iBaseCenter = (unsigned short) vertices.size();
        vertices.emplace_back(); // Base center vertex at (0, 0, -1)
        vertices.back().position = { 0.0f, 0.0f, -radius, 1.0f };

        const unsigned short iBaseStart = (unsigned short) vertices.size();
        for ( int iLong = 0; iLong < longDiv; iLong++ )
        {
            V vertex;
            vertex.position = { radius * std::cos( longitudeAngle * iLong ), radius * std::sin( longitudeAngle * iLong ), -radius, 1.0f };
            vertices.push_back( vertex );
        }

        // Base indices
        for ( unsigned short iLong = 0; iLong < longDiv; iLong++ )
        {
            indices.push_back( iBaseCenter );
            indices.push_back( ( iLong + 1 ) % longDiv + iBaseStart );
            indices.push_back( iLong + iBaseStart );
        }

        return { std::move( vertices ), std::move( indices ) };
    }

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};