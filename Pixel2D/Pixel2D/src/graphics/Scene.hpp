#pragma once
#include <entityx/entityx.h>
#include <Box2D/Dynamics/b2World.h>

//Components
#include "components/Render.hpp"
#include "components/Transform.hpp"

using namespace entityx;

namespace sf
{
	class RenderTarget;
}

namespace px
{
	class Scene : public EntityX
	{
	public:
		enum class Shapes
		{
			Circle,
			Rectangle
		};

	public:
		//Can't most of this stuff just be replaced by an entity?
		struct ObjectInfo
		{
			ObjectInfo(){}
			ObjectInfo(const std::string & name, const sf::Vector2f & pos, const sf::Vector2f & scale, 
					   const float & rot, const unsigned int & i, const bool & picked, const std::string & layer) :
					   pickedName(name), position(pos), scale(scale), rotation(rot), selected(i), picked(picked), layer(layer) {}

			void changeName(const std::string & name)
			{
				nameChanger.clear(); nameChanger.resize(50);

				for (unsigned p = 0; p < name.size(); ++p)
					nameChanger[p] = name[p];
			}

			std::string layer;
			unsigned int selected;
			bool picked = false;
			float rotation;
			std::string pickedName;
			sf::Vector2f position;
			sf::Vector2f scale;
			std::vector<char> nameChanger;
		};

	public:
		Scene(sf::RenderTarget & target, b2World* world);

	public:
		void createEntity(const Scene::Shapes & shape, const sf::Vector2f & position, const std::string & name, ObjectInfo & info);
		void destroyEntity(const std::string & name);
		void destroyEntities(const std::string & layer);
		void destroyEntities();
		void updateLayer(std::string & cName, const std::string & layer);
		void updateName(std::string & cName, const std::string & nName);
		void updateTransform(const ObjectInfo & info);
		void updateTransformSystem(const double & dt);
		void updateRenderSystem(const double & dt);
		bool checkIntersection(const sf::Vector2f & point, ObjectInfo & info);

	public:
		std::vector<std::string> & getLayers();
		Entity getEntity(const std::string & name);
		EntityManager & getEntities();
		
	private:
		b2World* m_world;
		std::vector<std::string> m_layers;
		EntityManager m_entities;
		EventManager m_events;
		SystemManager m_systems;
	};
}