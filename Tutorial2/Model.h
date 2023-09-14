#pragma once
#include "Drawable.h"
#include "BindableBase.h"
#include "VertexLayout.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <type_traits>
#include "ImGUI/imgui.h"

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
	struct PSMaterialConstantFullmonte {
		DirectX::XMFLOAT4 specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float specularWeight = 0.20f;
		float specularPower = 20.0f;
		BOOL specularMapEnabled = TRUE;

		BOOL normalMapEnabled = TRUE;
		BOOL  hasGlossMap = FALSE;
		float padding[3];
	};

	struct PSMaterialConstantNotex {
		DirectX::XMFLOAT4 materialColor = { 0.447970f, 0.327254f, 0.176283f, 1.0f };
		DirectX::XMFLOAT4 specularColor = { 0.65f, 0.65f, 0.65f, 1.0f };
		float specularPower;
		float padding[3];
	};
	
	Node( int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform ) noexcept;

	void Draw( Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform ) const noexcept;
	void SetAppliedTransform( DirectX::FXMMATRIX transform ) noexcept;
	void ShowTree( Node*& pSelectedNode ) const noexcept;

	template <class T>
	bool ControlsUI( Renderer& renderer, T& materialConst )
	{
		if ( meshPtrs.empty() ) return false;

		if constexpr ( std::is_same<T, PSMaterialConstantFullmonte>::value )
		{
			if ( auto pcb = meshPtrs.front()->SearchBindable<PixelConstantBuffer<T>>() )
			{
				ImGui::Text( "Material" );

				bool normalMapEnabled = (bool) materialConst.normalMapEnabled;
				ImGui::Checkbox( "Norm Map", &normalMapEnabled );
				materialConst.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

				bool specularMapEnabled = (bool) materialConst.specularMapEnabled;
				ImGui::Checkbox( "Spec Map", &specularMapEnabled );
				materialConst.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

				bool hasGlossMap = (bool) materialConst.hasGlossMap;
				ImGui::Checkbox( "Gloss Alpha", &hasGlossMap );
				materialConst.hasGlossMap = hasGlossMap ? TRUE : FALSE;

				ImGui::SliderFloat( "Spec Weight", &materialConst.specularWeight, 0.0f, 2.0f );

				ImGui::SliderFloat( "Spec Pow", &materialConst.specularPower, 0.0f, 1000.0f, "%f", 5.0f );

				ImGui::ColorPicker3( "Spec Color", reinterpret_cast<float*>( &materialConst.specularColor ) );

				pcb->Update( renderer, materialConst );
				return true;
			}
		}
		else if constexpr ( std::is_same<T, PSMaterialConstantNotex>::value )
		{
			if ( auto pcb = meshPtrs.front()->SearchBindable<PixelConstantBuffer<T>>() )
			{
				ImGui::Text( "Material" );

				ImGui::SliderFloat( "Spec Power", &materialConst.specularPower, 0.0f, 1000.0f, "%f", 5.0f );

				ImGui::ColorPicker3( "Spec Color", reinterpret_cast<float*>( &materialConst.specularColor ) );

				ImGui::ColorPicker3( "Diffuse Color", reinterpret_cast<float*>( &materialConst.materialColor ) );

				pcb->Update( renderer, materialConst );
				return true;
			}
		}
		
		return false;
	}

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
	void ShowWindow( Renderer& renderer, const char* windowName = nullptr ) noexcept;

	void SetRootTransform( DirectX::FXMMATRIX transform );

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