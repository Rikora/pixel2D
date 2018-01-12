#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <Kairos\Timestep.hpp>
#include <Kairos\FpsLite.hpp>

#include "Scene.hpp"
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

		struct Parenting
		{
			Parenting(const std::string & name, const bool & parented, const bool & parent, const unsigned int & index) : name(name), parented(parented), 
					  parent(parent), index(index) {}

			std::string name;
			std::vector<Parenting> children;
			unsigned int index;
			bool parented;
			bool parent;
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
		void listChildren(const unsigned int & index, std::vector<Parenting> & children);
		void updateLayerItem(int & item);
		void layerSettingsMenu();
		void sceneDock();
		void hierarchyDock();
		void inspectorDock();

	private:
		std::unique_ptr<utils::TileMap> m_tileMap;
		sf::RenderWindow m_window;
		sf::RenderTexture m_sceneTexture;
		sf::View m_sceneView;
		sf::Vector2i m_currentMousePos;
		sf::Vector2i m_previousMousePos;
		sf::Vector2i m_deltaMouse;
		sf::Vector2f m_worldPos;
		Scene::ObjectInfo m_objectInfo;
		bool m_isSceneHovered;

	private:
		TextureHolder m_textures;
		static int m_layerItem;
		static bool m_showLayerSettings;
		std::vector<Parenting> m_children;

	private:
		static std::unique_ptr<Scene> m_scene;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}