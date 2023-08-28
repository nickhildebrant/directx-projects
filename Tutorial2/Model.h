#pragma once
#include "Drawable.h"
#include "BindableBase.h"
#include "VertexLayout.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public Drawable {
public:
	Mesh( Renderer& renderer, std::vector<std::shared_ptr<Bindable>> bindPtrs );

	void Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node {
	friend class Model;

public:
	Node( int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform ) noexcept;

	void Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept;
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;
	void ShowTree( Node*& pSelectedNode ) const noexcept;

	int GetID() const;

private:
	int id;
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;

	void AddChild( std::unique_ptr<Node> pChild ) noexcept;
};

class Model {
public:
	Model( Renderer& renderer, const std::string fileName );
	~Model() noexcept;

	void Draw( Renderer& renderer ) const;
	void ShowWindow( const char* windowName = nullptr ) noexcept;

private:
	struct {
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} position;

	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;

	static std::unique_ptr<Mesh> ParseMesh( Renderer& renderer, const aiMesh& mesh, const aiMaterial* const* pMaterials ); // array of const aiMaterials <- const ptr <- ptr
	std::unique_ptr<Node> ParseNode( int& nextId, const aiNode& node ) noexcept;
};