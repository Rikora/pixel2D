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
	private:
		struct Object
		{
			void setPosition(float x, float y) { entity.component<Render>()->shape->setPosition(x, y); }
			void getEntity(const std::string & name){ entity = m_scene->getEntity(name); }
			const float getX() { return entity.component<Render>()->shape->getPosition().x; }
			const float getY() { return entity.component<Render>()->shape->getPosition().y; }

			Entity entity;
		};

	public:
		Core();
		~Core();

	public:
		void run();

	private:
		void initialize();
		void loadLua();
		void loadLuaScripts();
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
		Scene::ObjectInfo m_objectInfo;
		bool m_isSceneHovered;

	private:
		static std::unique_ptr<Scene> m_scene;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
		//tmx::Map m_map;
	};
}