#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <Kairos/Timestep.hpp>
#include <Kairos/FpsLite.hpp>

#include "Scene.hpp"
#include "../physics/Physics.hpp"
#include "../utils/ResourceHolder.hpp"
#include "../utils/ResourceIdentifiers.hpp"
#include "../utils/TileMap.hpp"

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
		void loadTextures();
		void loadLua();
		void loadLuaScripts();
		void render();
		void processEvents();
		void update();
		void updateGUI();

	private:
		void addLayer(std::vector<char> & layerHolder);
		void updateLayerItem(int & item);
		void layerSettingsMenu();
		void sceneDock();
		void hierarchyDock();
		void inspectorDock();
		void displayComponents();

	private:
		std::unique_ptr<utils::TileMap> m_tileMap;
		sf::RenderWindow m_window;
		sf::RenderTexture m_sceneTexture;
		sf::View m_sceneView;
		sf::Vector2i m_currentMousePos;
		sf::Vector2i m_previousMousePos;
		sf::Vector2i m_deltaMouse;
		sf::Vector2f m_worldPos;
		ObjectInfo m_objectInfo;
		bool m_isSceneHovered;

	private:
		TextureHolder m_textures;
		static int m_layerItem;
		static bool m_showLayerSettings;

	private:
		static std::unique_ptr<Scene> m_scene;
		std::unique_ptr<Physics> m_physicsWorld;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}