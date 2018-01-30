#pragma once
#include <entityx/entityx.h>
#include "../utils/ObjectInfo.hpp"

//Components
#include "components/Render.hpp"
#include "components/Transform.hpp"
#include "components/Rigidbody.hpp"

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