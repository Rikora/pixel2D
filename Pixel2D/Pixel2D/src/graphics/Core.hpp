#pragma once
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Window\Event.hpp>
#include <Kairos\Timestep.hpp>
#include <Kairos\FpsLite.hpp>

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

	private:
		sf::CircleShape m_circle;
		sf::Vector2f m_currentCirclePosition;
		sf::Vector2f m_previousCirclePosition;
		const float m_movementSpeed;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}