#pragma once
#include <set>

#include "GUIManager.h"
#include "Window.h"
#include "Timer.h"
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "TexturedBox.h"
#include "ModelTest.h"

#include "Camera.h"
#include "Light.h"

#include "VertexLayout.h"

class Application {
public:
	Application();
	~Application();

	int Run();

	void F()
	{
		//VertexLayout vertexLayout;
		//vertexLayout.Append<VertexLayout::Position3D>().Append<VertexLayout::Normal>();

		//VertexBuffer vertexBuffer( std::move( vertexLayout ) );
		//vertexBuffer.EmplaceBack( DirectX::XMFLOAT4 { 1.0f, 1.0f, 5.0f, 1.0f }, DirectX::XMFLOAT4 { 2.0f, 1.0f, 4.0f, 0.0f } );

		VertexBuffer vertexBuffer( 
			std::move( VertexLayout{}.Append<VertexLayout::Position3D>().Append<VertexLayout::Normal>().Append<VertexLayout::Texture2D>() ) 
		);

		vertexBuffer.EmplaceBack( 
			DirectX::XMFLOAT4{ 1.0f, 1.0f, 5.0f, 1.0f },
			DirectX::XMFLOAT4{ 2.0f, 1.0f, 4.0f, 0.0f },
			DirectX::XMFLOAT2{ 6.0f, 9.0f }
		);

		vertexBuffer.EmplaceBack(
			DirectX::XMFLOAT4{ 6.0f, 9.0f, 6.0f, 1.0f },
			DirectX::XMFLOAT4{ 9.0f, 6.0f, 9.0f, 0.0f },
			DirectX::XMFLOAT2{ 4.2f, 0.0f }
		);

		auto position = vertexBuffer[0].Attr<VertexLayout::Position3D>();
		auto normal = vertexBuffer[0].Attr<VertexLayout::Normal>();
		auto tex = vertexBuffer[1].Attr<VertexLayout::Texture2D>();

		vertexBuffer.Back().Attr<VertexLayout::Position3D>().z = 420.0f;
		position = vertexBuffer.Back().Attr<VertexLayout::Position3D>();
	}

	class ModelFactory {
	public:
		ModelFactory(Renderer& renderer) : renderer(renderer) {}

		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT4 mat{ cdist( rng ), cdist( rng ), cdist( rng ), 1.0f };

			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Box>( renderer, rng, adist, ddist, odist, rdist, bdist, mat );

			case 1:
				return std::make_unique<Cylinder>( renderer, rng, adist, ddist, odist, rdist, bdist, tdist );

			case 2:
				return std::make_unique<Pyramid>( renderer, rng, adist, ddist, odist, rdist, tdist );

			case 3:
				return std::make_unique<TexturedBox>( renderer, rng, adist, ddist, odist, rdist );

			case 4:
				return std::make_unique<ModelTest>( renderer, rng, adist, ddist, odist, rdist, mat, 1.5f );

			default:
				assert(false && "No available Types");
				return {};
			}
		}

	private:
		const float PI = 3.14159265f;

		Renderer& renderer;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		std::uniform_int_distribution<int> tdist{ 3, 30 };
		std::uniform_int_distribution<int> latdist{ 5, 20 };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_real_distribution<float> cdist{ 0.0f, 1.0f };
		std::uniform_int_distribution<int> typedist{ 0, 4 };
	};

	void SpawnSimulationWindow();
	void SpawnBoxManagerWindow();
	void SpawnBoxWindows();

private:
	GUIManager m_guiManager;

	Window m_window;
	Timer m_timer;

	Camera camera;
	Light light;

	bool showUI = true;
	float simulationSpeed = 1.0f;

	std::vector<std::unique_ptr<class Drawable>> m_models;
	std::vector<class Box*> boxes;

	std::optional<int> comboBoxIndex;
	std::set<int> boxIDs;

	void RenderFrame();
};