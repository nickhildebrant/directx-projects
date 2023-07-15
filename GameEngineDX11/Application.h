#pragma once
#include "Window.h"
#include "Timer.h"
#include "Box.h"

class Application {
public:
	Application();
	~Application();

	int Run();

	class ModelFactory {
	public:
		ModelFactory( Renderer& renderer ) : renderer( renderer ) {}

		std::unique_ptr<Drawable> operator()()
		{
			switch ( typedist( rng ) )
			{
			case 0:
				return std::make_unique<Box>( renderer, rng, adist, ddist, odist, rdist, bdist );
				break;

			default:
				assert( false && "No available Types" );
				return {};
				break;
			}
		}

	private:
		const float PI = 3.14159265f;

		Renderer& renderer;
		std::mt19937 rng{ std::random_device{}( ) };
		std::uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		std::uniform_int_distribution<int> latdist{ 5, 20 };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_int_distribution<int> typedist{ 0, 2 };
	};

private:
	Window m_window;
	Timer m_timer;

	std::vector<std::unique_ptr<class Drawable>> m_models;

	void DoFrame();
};