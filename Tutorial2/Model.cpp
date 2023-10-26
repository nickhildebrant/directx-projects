#include "Model.h"
#include "ImGUI/imgui.h"
#include "Surface.h"
#include "BindableBase.h"

#include <unordered_map>

// Mesh
Mesh::Mesh( Renderer& renderer, std::vector<std::shared_ptr<Bindable>> bindPtrs )
{
	AddBind( Topology::Resolve( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	for ( auto& pBindable : bindPtrs )
	{
		AddBind( std::move( pBindable ) );
	}

	AddBind( std::make_shared<TransformConstantBuffer>( renderer, *this ) );
}

void Mesh::Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept
{
	DirectX::XMStoreFloat4x4( &transform, accumulatedTransform );
	Drawable::Draw( renderer );
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4( &transform );
}

// Node
Node::Node( int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& trans ) noexcept : id( id ), meshPtrs( std::move( meshPtrs ) ), name( name )
{
	DirectX::XMStoreFloat4x4( &transform, trans );
	DirectX::XMStoreFloat4x4( &appliedTransform, DirectX::XMMatrixIdentity() );
}

void Node::Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept
{
	const DirectX::XMMATRIX built = DirectX::XMLoadFloat4x4( &appliedTransform ) * DirectX::XMLoadFloat4x4( &transform ) * accumulatedTransform;
	for ( const Mesh* pMesh : meshPtrs )
	{
		pMesh->Draw( renderer, built );
	}

	for ( const auto& pChild : childPtrs )
	{
		pChild->Draw( renderer, built );
	}
}

void Node::AddChild( std::unique_ptr<Node> pChild ) noexcept
{
	assert( pChild );
	childPtrs.push_back( std::move( pChild ) );
}

void Node::ShowTree( Node*& pSelectedNode ) const noexcept
{
	// if there is no selected node, set selectedId to an impossible value
	const int selectedId = ( pSelectedNode == nullptr ) ? -1 : pSelectedNode->GetID();

	// build up flags for current node
	const int node_flags = ImGuiTreeNodeFlags_OpenOnArrow 
		| ( ( GetID() == selectedId ) ? ImGuiTreeNodeFlags_Selected : 0 )
		| ( ( childPtrs.size() == 0 ) ? ImGuiTreeNodeFlags_Leaf : 0 );

	// render this node
	const bool expanded = ImGui::TreeNodeEx( (void*) (intptr_t) GetID(), node_flags, name.c_str() );

	// processing for selecting node
	if ( ImGui::IsItemClicked() )
	{
		pSelectedNode = const_cast<Node*>( this );
	}

	// recursive rendering of open node's children
	if ( expanded )
	{
		for ( const auto& pChild : childPtrs )
		{
			pChild->ShowTree( pSelectedNode );
		}

		ImGui::TreePop();
	}
}

void Node::SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept
{
	DirectX::XMStoreFloat4x4( &appliedTransform, transform );
}

int Node::GetID() const { return id; }

// Model
class ModelWindow {
public:
	void Show( Renderer& renderer, const char* windowName, const Node& root ) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";

		int nodeIndexTracker = 0;
		if ( ImGui::Begin( windowName ) )
		{
			ImGui::Columns( 2, nullptr, true );
			root.ShowTree( pSelectedNode );

			ImGui::NextColumn();
			if ( pSelectedNode != nullptr )
			{
				auto& transform = transforms[pSelectedNode->GetID()];
				ImGui::Text( "Orientation" );
				ImGui::SliderAngle( "Roll", &transform.roll, -180.0f, 180.0f );
				ImGui::SliderAngle( "Pitch", &transform.pitch, -180.0f, 180.0f );
				ImGui::SliderAngle( "Yaw", &transform.yaw, -180.0f, 180.0f );

				ImGui::Text( "Position" );
				ImGui::SliderFloat( "X", &transform.x, -20.0f, 20.0f );
				ImGui::SliderFloat( "Y", &transform.y, -20.0f, 20.0f );
				ImGui::SliderFloat( "Z", &transform.z, -20.0f, 20.0f );
				
				if ( !pSelectedNode->ControlsUI( renderer, skinMaterial ) )
				{
					pSelectedNode->ControlsUI( renderer, ringMaterial );
				}
			}
		}
		ImGui::End();
	}

	DirectX::XMMATRIX GetTransform() const noexcept
	{
		assert( pSelectedNode != nullptr );
		const auto& transform = transforms.at( pSelectedNode->GetID() );
		return DirectX::XMMatrixRotationRollPitchYaw( transform.roll, transform.pitch, transform.yaw ) *
			DirectX::XMMatrixTranslation( transform.x, transform.y, transform.z );
	}

	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}

private:
	Node* pSelectedNode;
	Node::PSMaterialConstantFullmonte skinMaterial;
	Node::PSMaterialConstantNotex ringMaterial;

	struct TransformParameters {
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	std::unordered_map<int, TransformParameters> transforms;
};

Model::Model( Renderer& renderer, const std::string& filepath, float scale ) : pWindow( std::make_unique<ModelWindow>() )
{
	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile( filepath.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	for ( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( renderer, *pScene->mMeshes[i], pScene->mMaterials, filepath, scale ) );
	}

	int nextId = 0;
	pRoot = ParseNode( nextId, *pScene->mRootNode );
}

Model::~Model() noexcept {}

void Model::Draw( Renderer& renderer ) const
{
	if ( Node* node = pWindow->GetSelectedNode() )
	{
		node->SetAppliedTransform( pWindow->GetTransform() );
	}

	pRoot->Draw( renderer, DirectX::XMMatrixIdentity() );
}

void Model::ShowWindow( Renderer& renderer, const char* windowName ) noexcept
{
	pWindow->Show( renderer, windowName, *pRoot );
}

void Model::SetRootTransform( DirectX::FXMMATRIX transform )
{
	pRoot->SetAppliedTransform( transform );
}

std::unique_ptr<Mesh> Model::ParseMesh( Renderer& renderer, const aiMesh& mesh, 
	const aiMaterial* const* pMaterials, const std::filesystem::path& path, float scale )
{
	using VertexHandler::VertexLayout;

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	const std::string folder_path = path.parent_path().string() + "/";

	bool hasSpecular = false;
	bool hasAlphaGloss = false;
	bool hasNormal = false;
	bool hasDiffuse = false;
	float shininess = 20.0f;
	DirectX::XMFLOAT4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT4 diffuseColor = { 0.50f, 0.50f, 0.75f, 1.0f };
	if ( mesh.mMaterialIndex >= 0 )
	{
		const aiMaterial& material = *pMaterials[mesh.mMaterialIndex];

		aiString textureFileName;

		if ( material.GetTexture( aiTextureType_DIFFUSE, 0, &textureFileName ) == aiReturn_SUCCESS )
		{
			bindablePtrs.push_back( Texture::Resolve( renderer, folder_path + std::string( textureFileName.C_Str() ) ) );
			hasDiffuse = true;
		}
		else
		{
			material.Get( AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>( diffuseColor ) );
		}

		if ( material.GetTexture( aiTextureType_SPECULAR, 0, &textureFileName ) == aiReturn_SUCCESS )
		{
			std::string filename = folder_path + std::string( textureFileName.C_Str() );
			std::shared_ptr<Texture> pTexture = Texture::Resolve( renderer, folder_path + std::string( textureFileName.C_Str() ), 1u );
			hasAlphaGloss = pTexture->HasAlpha();
			bindablePtrs.push_back( std::move( pTexture ) );
			hasSpecular = true;
		}
		else
		{
			material.Get( AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>( specularColor ) );
		}

		if ( !hasAlphaGloss )
		{
			material.Get( AI_MATKEY_SHININESS, shininess );
		}

		if ( material.GetTexture( aiTextureType_NORMALS, 0, &textureFileName ) == aiReturn_SUCCESS )
		{
			std::shared_ptr<Texture> pTexture = Texture::Resolve( renderer, folder_path + std::string( textureFileName.C_Str() ), 2u );
			hasAlphaGloss = pTexture->HasAlpha();
			bindablePtrs.push_back( std::move( pTexture ) );
			hasNormal = true;
		}

		if ( hasDiffuse || hasSpecular || hasNormal )
		{
			bindablePtrs.push_back( Sampler::Resolve( renderer ) );
		}
	}

	std::string meshTag = path.string() + "%" + mesh.mName.C_Str();

	if ( hasDiffuse && hasNormal && hasSpecular )
	{
		VertexHandler::VertexBuffer vertexBuffer( std::move( VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Tangent )
			.Append( VertexLayout::Bitangent )
			.Append( VertexLayout::Texture2D )
		) );

		for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vertexBuffer.EmplaceBack(
				DirectX::XMFLOAT4{ mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale, 1.0f },
				DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f },
				DirectX::XMFLOAT4{ mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z, 0.0f },
				DirectX::XMFLOAT4{ mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z, 0.0f },
				DirectX::XMFLOAT2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const aiFace face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[2] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[0] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( renderer, meshTag, vertexBuffer ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( renderer, meshTag, indices ) );

		auto pvs = VertexShader::Resolve( renderer, "NormalPhongVS.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( renderer, "NormalSpecularPS.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( renderer, vertexBuffer.GetLayout(), pvsbc ) );

		Node::PSMaterialConstantFullmonte materialConstant;
		materialConstant.specularPower = shininess;
		materialConstant.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;

		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( PixelConstantBuffer<Node::PSMaterialConstantFullmonte>::Resolve( renderer, materialConstant, 1u ) );
	}
	else if ( hasDiffuse && hasNormal )
	{
		VertexHandler::VertexBuffer vertexBuffer( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Tangent )
			.Append( VertexLayout::Bitangent )
			.Append( VertexLayout::Texture2D )
		) );

		for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vertexBuffer.EmplaceBack(
				DirectX::XMFLOAT4{ mesh.mVertices[i].x* scale, mesh.mVertices[i].y* scale, mesh.mVertices[i].z* scale, 1.0f },
				DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f },
				DirectX::XMFLOAT4{ mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z, 0.0f },
				DirectX::XMFLOAT4{ mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z, 0.0f },
				DirectX::XMFLOAT2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[2] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[0] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( renderer, meshTag, vertexBuffer ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( renderer, meshTag, indices ) );

		auto pvs = VertexShader::Resolve( renderer, "NormalPhongVS.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( renderer, "NormalPhongPS.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( renderer, vertexBuffer.GetLayout(), pvsbc ) );

		struct PSMaterialConstantDiffnorm
		{
			float specularIntensity;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding;
		} materialConstant;

		materialConstant.specularIntensity = ( specularColor.x + specularColor.y + specularColor.z ) / 3.0f;
		materialConstant.specularPower = shininess;

		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiffnorm>::Resolve( renderer, materialConstant, 1u ) );
	}
	else if ( hasDiffuse && !hasNormal && hasSpecular )
	{
		VertexHandler::VertexBuffer vertexBuffer( std::move( VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Texture2D )
		) );

		for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vertexBuffer.EmplaceBack(
				DirectX::XMFLOAT4{ mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale, 1.0f },
				DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f },
				DirectX::XMFLOAT2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const aiFace face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[2] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[0] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( renderer, meshTag, vertexBuffer ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( renderer, meshTag, indices ) );

		std::shared_ptr<VertexShader> pvs = VertexShader::Resolve( renderer, "PhongVertexShader.cso" );
		ID3DBlob* pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		//bindablePtrs.push_back( PixelShader::Resolve( renderer, "PhongSpecPS.cso" ) );
		bindablePtrs.push_back( PixelShader::Resolve( renderer, "TexturePhongPixelShader.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( renderer, vertexBuffer.GetLayout(), pvsbc ) );

		struct PSMaterialConstantDiffuseSpec {
			float specularPowerConst;
			//BOOL  hasGloss = TRUE;
			float specularMapWeight;
			float padding[2];
		} psConst;

		psConst.specularPowerConst = shininess;
		//psConst.hasGloss = hasAlphaGloss ? TRUE : FALSE;
		psConst.specularMapWeight = 1.0f;

		bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiffuseSpec>::Resolve( renderer, psConst, 1u ) );
	}
	else if ( hasDiffuse )
	{
		VertexHandler::VertexBuffer vertexBuffer( std::move( VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Texture2D )
		) );

		for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vertexBuffer.EmplaceBack(
				DirectX::XMFLOAT4{ mesh.mVertices[i].x* scale, mesh.mVertices[i].y* scale, mesh.mVertices[i].z* scale, 1.0f },
				DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f },
				DirectX::XMFLOAT2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const aiFace face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[2] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[0] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( renderer, meshTag, vertexBuffer ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( renderer, meshTag, indices ) );

		auto pvs = VertexShader::Resolve( renderer, "TexturePhongVertexShader.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( renderer, "TexturePhongPixelShader.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( renderer, vertexBuffer.GetLayout(), pvsbc ) );

		struct PSMaterialConstantDiffuse
		{
			float specularIntensity;
			float specularPower;
			float padding[2];
		} materialConstant;

		materialConstant.specularIntensity = ( specularColor.x + specularColor.y + specularColor.z ) / 3.0f;
		materialConstant.specularPower = shininess;

		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( PixelConstantBuffer<PSMaterialConstantDiffuse>::Resolve( renderer, materialConstant, 1u ) );
	}
	else if ( !hasDiffuse && !hasNormal && !hasSpecular )
	{
		VertexHandler::VertexBuffer vertexBuffer( std::move( VertexLayout{}
			.Append( VertexLayout::Position3D ) 
			.Append( VertexLayout::Normal ) 
		) );

		for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vertexBuffer.EmplaceBack(
				DirectX::XMFLOAT4{ mesh.mVertices[i].x* scale, mesh.mVertices[i].y* scale, mesh.mVertices[i].z* scale, 1.0f },
				DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f }
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[2] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[0] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( renderer, meshTag, vertexBuffer ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( renderer, meshTag, indices ) );

		auto pvs = VertexShader::Resolve( renderer, "PhongVS.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( renderer, "PhongPS.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( renderer, vertexBuffer.GetLayout(), pvsbc ) );
		
		Node::PSMaterialConstantNotex materialConstant;
		materialConstant.materialColor = diffuseColor;
		materialConstant.specularColor = specularColor;
		materialConstant.specularPower = shininess;

		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( PixelConstantBuffer<Node::PSMaterialConstantNotex>::Resolve( renderer, materialConstant, 1u ) );
	}
	else
	{
		throw std::runtime_error( "terrible combination of textures in material smh" );
	}

	return std::make_unique<Mesh>( renderer, std::move( bindablePtrs ) );
}

std::unique_ptr<Node> Model::ParseNode( int& nextId, const aiNode& node ) noexcept
{
	const DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( reinterpret_cast<const DirectX::XMFLOAT4X4*>( &node.mTransformation ) ) );

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve( node.mNumMeshes );
	for ( size_t i = 0; i < node.mNumMeshes; i++ )
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
	}

	auto pNode = std::make_unique<Node>( nextId++, node.mName.C_Str(), std::move( curMeshPtrs ), transform );
	for ( size_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( nextId, *node.mChildren[i] ) );
	}

	return pNode;
}