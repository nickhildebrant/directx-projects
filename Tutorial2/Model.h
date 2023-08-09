#pragma once
#include "DrawableBase.h"
#include "BindableBase.h"
#include "VertexLayout.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <optional>

class Mesh : public DrawableBase<Mesh> {
public:
	Mesh( Renderer& renderer, std::vector<std::unique_ptr<Bindable>> bindPtrs );

	void Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node {
	friend class Model;
	friend class ModelWindow;

public:
	Node( const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform ) noexcept;

	void Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept;
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;

private:
	std::string name;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;

	void AddChild( std::unique_ptr<Node> pChild ) noexcept;
	void ShowTree( int& nodeIndex, std::optional<int>& selectedIndex, Node*& pSelectedNode ) const noexcept;
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

	static std::unique_ptr<Mesh> ParseMesh( Renderer& renderer, const aiMesh& mesh );
	std::unique_ptr<Node> ParseNode( const aiNode& node ) noexcept;
};