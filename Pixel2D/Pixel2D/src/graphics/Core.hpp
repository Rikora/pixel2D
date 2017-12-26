#pragma once
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
		struct Object
		{
			void getEntity(const std::string name){ entity = m_scene->getEntity(name); }
			const float getX() { return entity.component<Render>()->shape->getPosition().x; }
			const float getY() { return entity.component<Render>()->shape->getPosition().y; }
			void setPosition(float x, float y) { entity.component<Render>()->shape->setPosition(x, y); }

			Entity entity;
		};

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
		sf::Vector2i m_currentMousePos;
		sf::Vector2i m_previousMousePos;
		bool m_isSceneHovered;

	private:
		static std::unique_ptr<Scene> m_scene;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}