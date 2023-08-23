#include "Model.h"
#include "ImGUI/imgui.h"

#include <unordered_map>

// Mesh
Mesh::Mesh( Renderer& renderer, std::vector<std::unique_ptr<Bindable>> bindPtrs )
{
	if ( !IsStaticInitialized() )
	{
		AddStaticBind( std::make_unique<Topology>( renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}

	for ( auto& pBindable : bindPtrs )
	{
		if ( auto pIndexBuffer = dynamic_cast<IndexBuffer*>( pBindable.get() ) )
		{
			AddIndexBuffer( std::unique_ptr<IndexBuffer>{ pIndexBuffer } );
			pBindable.release();
		}
		else
		{
			AddBind( std::move( pBindable ) );
		}
	}

	AddBind( std::make_unique<TransformConstantBuffer>( renderer, *this ) );
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
	void Show( const char* windowName, const Node& root ) noexcept
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

Model::Model( Renderer& renderer, const std::string fileName ) : pWindow( std::make_unique<ModelWindow>() )
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile( fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals
	);

	for ( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( renderer, *pScene->mMeshes[i] ) );
	}

	int nextId = 0;
	pRoot = ParseNode( nextId, *pScene->mRootNode );
}

Model::~Model() noexcept {}

void Model::Draw( Renderer& renderer ) const
{
	if ( auto node = pWindow->GetSelectedNode() )
	{
		node->SetAppliedTransform( pWindow->GetTransform() );
	}

	pRoot->Draw( renderer, DirectX::XMMatrixIdentity() );
}

void Model::ShowWindow( const char* windowName ) noexcept
{
	pWindow->Show( windowName, *pRoot );
}

std::unique_ptr<Mesh> Model::ParseMesh( Renderer& renderer, const aiMesh& mesh )
{
	using VertexHandler::VertexLayout;

	VertexHandler::VertexBuffer vertexBuffer( std::move(
		VertexLayout{}
		.Append( VertexLayout::Position3D )
		.Append( VertexLayout::Normal )
	) );

	for ( unsigned int i = 0; i < mesh.mNumVertices; i++ )
	{
		vertexBuffer.EmplaceBack(
			DirectX::XMFLOAT4{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z, 1.0f },
			DirectX::XMFLOAT4{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z, 0.0f }
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve( mesh.mNumFaces * 3 );
	for ( unsigned int i = 0; i < mesh.mNumFaces; i++ )
	{
		const aiFace& face = mesh.mFaces[i];
		assert( face.mNumIndices == 3 );
		indices.push_back( face.mIndices[0] );
		indices.push_back( face.mIndices[1] );
		indices.push_back( face.mIndices[2] );
	}

	std::vector<std::unique_ptr<Bindable>> bindablePtrs;

	bindablePtrs.push_back( std::make_unique<VertexBuffer>( renderer, vertexBuffer ) );

	bindablePtrs.push_back( std::make_unique<IndexBuffer>( renderer, indices ) );

	auto pvs = std::make_unique<VertexShader>( renderer, L"PhongVertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back( std::move( pvs ) );

	bindablePtrs.push_back( std::make_unique<PixelShader>( renderer, L"PhongPixelShader.cso" ) );

	bindablePtrs.push_back( std::make_unique<InputLayout>( renderer, vertexBuffer.GetLayout().GetD3DLayout(), pvsbc ) );

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color = { 0.6f, 0.6f, 0.8f, 1.0f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} materialConstant;

	bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( renderer, materialConstant, 1u ) );

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