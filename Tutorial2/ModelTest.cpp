#include "ModelTest.h"
#include "BindableBase.h"
#include "RendererErrorMacros.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelTest::ModelTest( Renderer& renderer, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, DirectX::XMFLOAT4 material, float scale )
	:
	TestPoly( renderer, rng, adist, ddist, odist, rdist )
{
	if ( !IsStaticInitialized() )
	{
		struct Vertex
		{
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 normal;
		};

		Assimp::Importer importer;
		const auto pModel = importer.ReadFile( "../Models/suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );
		const auto pMesh = pModel->mMeshes[0];

		std::vector<Vertex> vertices;
		vertices.reserve( pMesh->mNumVertices );
		for ( unsigned int i = 0; i < pMesh->mNumVertices; i++ )
		{
			Vertex meshVertex;
			meshVertex.position = { pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale, 1.0f };
			meshVertex.normal = { pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z, 0.0f };

			vertices.push_back( meshVertex );


			//vertices.push_back
			//( {
			//	{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale },
			//	*reinterpret_cast<DirectX::XMFLOAT4*>( &pMesh->mNormals[i] )
			//	}
			//);
		}

		std::vector<unsigned short> indices;
		indices.reserve( pMesh->mNumFaces * 3 );
		for ( unsigned int i = 0; i < pMesh->mNumFaces; i++ )
		{
			const aiFace& face = pMesh->mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		AddStaticBind( std::make_unique<VertexBuffer>( renderer, vertices ) );

		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( renderer, indices ) );

		auto pvs = std::make_unique<VertexShader>( renderer, L"PhongVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( renderer, L"PhongPixelShader.cso" ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind( std::make_unique<InputLayout>( renderer, ied, pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT4 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} materialConstant;

		materialConstant.color = material;
		AddStaticBind( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( renderer, materialConstant, 1u ) );
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformConstantBuffer>( renderer, *this ) );
}