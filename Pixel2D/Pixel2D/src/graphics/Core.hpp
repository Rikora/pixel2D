#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <Kairos\Timestep.hpp>
#include <Kairos\FpsLite.hpp>

#include "Scene.hpp"
#include "../utils/ResourceHolder.hpp"
#include "../utils/ResourceIdentifiers.hpp"

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
		void updateLayerItem(int & item);
		void layerSettingsMenu();
		void sceneDock();
		void hierarchyDock();
		void inspectorDock();

	private:
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

		struct Parenting
		{
			Parenting(const std::string & name, const bool & parented, const bool & parent) : name(name), parented(parented), parent(parent) {}

			std::string name;
			std::vector<std::string> children;
			bool parented;
			bool parent;
		};

		std::vector<Parenting> m_children;

	private:
		static std::unique_ptr<Scene> m_scene;

	private:
		kairos::FpsLite m_fps;
		kairos::Timestep m_timestep;
	};
}