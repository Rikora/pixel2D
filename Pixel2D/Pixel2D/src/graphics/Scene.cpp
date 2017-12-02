#include "Scene.hpp"
#include <SFML\Graphics\CircleShape.hpp>

//Components
#include "components\Render.hpp"

//Systems
#include "systems\RenderSystem.hpp"

namespace px
{
	Scene::Scene(sf::RenderTarget & target) : m_entities(m_events), m_systems(m_entities, m_events)
	{
		//Basic entity
		auto entity = m_entities.create();
		
		//Circle
		auto shape = std::make_unique<sf::CircleShape>(10.f);
		shape->setFillColor(sf::Color::Yellow);
		shape->setOrigin(10.f, 10.f);
		shape->setPosition(sf::Vector2f(500.f, 233.f));

		//Apply components
		entity.assign<Render>(std::move(shape));

		//Systems
		m_systems.add<RenderSystem>(target);
		m_systems.configure();
	}

	Scene::~Scene()
	{
		destroyEntities();
	}

	void Scene::destroyEntities()
	{
		ComponentHandle<Render> render;

		for (Entity & entity : m_entities.entities_with_components(render))
			entity.destroy();
	}

	void Scene::updateSystems(double dt)
	{
		m_systems.update<RenderSystem>(dt);
	}

	EntityManager & Scene::getEntities()
	{
		return m_entities;
	}
}