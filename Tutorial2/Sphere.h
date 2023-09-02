#pragma once
#include <optional>
#include <DirectXMath.h>
#include "IndexedTriangleList.h"
#include "VertexLayout.h"

class Sphere {
public:
    static IndexedTriangleList MakeTesselated( VertexHandler::VertexLayout layout, int latitudeDivisions, int longitudeDivisions )
    {
        const float PI = 3.14159265f;

        if( latitudeDivisions < 3 ) latitudeDivisions = 3;
        if( longitudeDivisions < 3 ) longitudeDivisions = 3;

        constexpr float radius = 1.0f;
        const float latitudeAngle = PI / latitudeDivisions;
        const float longitudeAngle = 2.0f * PI / longitudeDivisions;

        VertexHandler::VertexBuffer vertexBuffer { std::move( layout ) };
        std::vector<unsigned short> indices;

        // Generate vertices for the sphere
        for ( int iLatitude = 0; iLatitude <= latitudeDivisions; iLatitude++ )
        {
            const float theta = iLatitude * latitudeAngle;
            const float sinTheta = std::sin( theta );
            const float cosTheta = std::cos( theta );

            for ( int iLongitude = 0; iLongitude <= longitudeDivisions; iLongitude++ )
            {
                const float phi = iLongitude * longitudeAngle;
                const float sinPhi = std::sin( phi );
                const float cosPhi = std::cos( phi );

                DirectX::XMFLOAT4 calculatedPosition;
                calculatedPosition.x = radius * sinTheta * cosPhi;
                calculatedPosition.y = radius * cosTheta;
                calculatedPosition.z = radius * sinTheta * sinPhi;
                calculatedPosition.w = 1.0f;
                vertexBuffer.EmplaceBack( calculatedPosition );
            }
        }

        // Generate indices for the sphere
        for ( unsigned short iLatitude = 0; iLatitude < latitudeDivisions; iLatitude++ )
        {
            for ( unsigned short iLongitude = 0; iLongitude < longitudeDivisions; iLongitude++ )
            {
                unsigned short topLeft = iLatitude * ( longitudeDivisions + 1 ) + iLongitude;
                unsigned short topRight = topLeft + 1;
                unsigned short bottomLeft = ( iLatitude + 1 ) * ( longitudeDivisions + 1 ) + iLongitude;
                unsigned short bottomRight = bottomLeft + 1;

                indices.push_back( topLeft );
                indices.push_back( bottomLeft );
                indices.push_back( topRight );

                indices.push_back( topRight );
                indices.push_back( bottomLeft );
                indices.push_back( bottomRight );
            }
        }

        return { std::move( vertexBuffer ), std::move( indices ) };
    }

    static IndexedTriangleList Make( std::optional<VertexHandler::VertexLayout> layout = std::nullopt )
	{
        using Element = VertexHandler::VertexLayout::ElementType;
        if ( !layout ) layout = VertexHandler::VertexLayout{}.Append( Element::Position3D );

		return MakeTesselated( std::move( *layout ), 12, 24);
	}
};