#pragma once
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <Kairos\Timestep.hpp>
#include <Kairos\FpsLite.hpp>

#include "Scene.hpp"

namespace px
{
	class Core
	{
	public:
		Core();
		~Core();

	public:
		void initialize();
		void run();
		void render();
		void processEvents();
		void update();
		void updateGUI();

	private:
		sf::RenderWindow m_window;
		sf::RenderTexture m_sceneTexture;
		sf::View m_sceneView;
		sf::Vector2f m_currentViewPosition;
		sf::Vector2f m_previousViewPosition;

	private:
		std::unique_ptr<Scene> m_scene;

	private:
		sf::CircleShape m_circle;
		const float m_viewSpeed;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}