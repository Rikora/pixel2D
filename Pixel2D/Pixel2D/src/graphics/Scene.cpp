#include <graphics/Scene.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <utils/Utility.hpp>

//Systems
#include <graphics/systems/RenderSystem.hpp>
#include <graphics/systems/TransformSystem.hpp>

namespace px
{
	Scene::Scene(sf::RenderTarget & target, b2World* world) : m_entities(m_events), m_systems(m_entities, m_events), m_world(world)
	{
		//Basic entity
		auto entity = m_entities.create();
		Transform transform(sf::Vector2f(500.f, 233.f), sf::Vector2f(1.f, 1.f), 0.f);

		//Circle
		auto shape = std::make_unique<sf::CircleShape>(10.f);
		shape->setFillColor(sf::Color::Red);
		utils::centerOrigin(*shape);
		shape->setPosition(transform.position);

		//Circle collider
		auto rigidbody = std::make_unique<RigidbodyShape>(RigidbodyShape::Collider::Circle, m_world);
		rigidbody->setTransform(sf::Vector2f(0.f, 0.f), 10.f, 0.f);

		//Apply components
		entity.assign<Render>(std::move(shape), "Circle", "Default");
		entity.assign<Transform>(transform);
		entity.assign<Rigidbody>(std::move(rigidbody));

		//Layers
		m_layers = { "Default", "Grass" };

		//Systems
		m_systems.add<RenderSystem>(target, m_layers);
		m_systems.add<TransformSystem>();
		m_systems.configure();
	}

	void Scene::createEntity(const Scene::Shapes & shape, const sf::Vector2f & position, const std::string & name, ObjectInfo & info)
	{
		//Create default shaped entities
		if (shape == Shapes::Circle)
		{
			auto entity = m_entities.create();
			auto shape = std::make_unique<sf::CircleShape>(5.f);

			//Transform
			Transform transform(position, sf::Vector2f(1.f, 1.f), 0.f);
			shape->setFillColor(sf::Color::Red);
			utils::centerOrigin(*shape);
			shape->setPosition(transform.position);
			shape->setScale(transform.scale);
			shape->setRotation(transform.rotation);

			//Apply components
			entity.assign<Render>(std::move(shape), name, "Default");
			entity.assign<Transform>(transform);

			//Update the GUI display
			info = { name, utils::circleCounter, true, entity };
			info.changeName(name);
		}
		else if (shape == Shapes::Rectangle)
		{
			auto entity = m_entities.create();
			auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(16.f, 16.f));

			//Transform
			Transform transform(position, sf::Vector2f(1.f, 1.f), 0.f);
			shape->setFillColor(sf::Color::Red);
			utils::centerOrigin(*shape);
			shape->setPosition(transform.position);
			shape->setScale(transform.scale);
			shape->setRotation(transform.rotation);

			//Apply components
			entity.assign<Render>(std::move(shape), name, "Default");
			entity.assign<Transform>(transform);

			//Update the GUI display
			info = { name, utils::rectangleCounter, true, entity };
			info.changeName(name);
		}
	}

	void Scene::destroyEntity(const std::string & name)
	{
		ComponentHandle<Render> render;

		//Remove entity which corresponds to the name
		for (Entity & entity : m_entities.entities_with_components(render))
		{
			if (render->name == name)
			{
				if (entity.has_component<Rigidbody>())
					entity.component<Rigidbody>()->body->destroyBody();
				entity.destroy();
				return;
			}
		}
	}

	void Scene::destroyEntities(const std::string & layer)
	{
		ComponentHandle<Render> render;

		//Remove entity which corresponds to the layer
		for (Entity & entity : m_entities.entities_with_components(render))
		{
			if (render->layer == layer)
			{
				if (entity.has_component<Rigidbody>())
					entity.component<Rigidbody>()->body->destroyBody();
				entity.destroy();
			}
		}
	}

	void Scene::destroyEntities()
	{
		ComponentHandle<Render> render;

		for (Entity & entity : m_entities.entities_with_components(render))
		{
			if (entity.has_component<Rigidbody>())
				entity.component<Rigidbody>()->body->destroyBody();
			entity.destroy();
		}
	}

	void Scene::updateName(std::string & cName, const std::string & nName)
	{
		ComponentHandle<Render> render;
		
		//Make sure that the new name is not already taken
		for (Entity & entity : m_entities.entities_with_components(render))
			if (render->name == nName)
				return;

		//Assign new name
		for (Entity & entity : m_entities.entities_with_components(render))
		{
			if (render->name == cName)
			{
				render->name = nName;
				cName = nName;
				return;
			}
		}
	}

	void Scene::updateTransformSystem(const double & dt)
	{
		m_systems.update<TransformSystem>(dt);
	}

	void Scene::updateRenderSystem(const double & dt)
	{
		m_systems.update<RenderSystem>(dt);
	}

	bool Scene::checkIntersection(const sf::Vector2f & point, ObjectInfo & info)
	{
		ComponentHandle<Render> render;
		ComponentHandle<Transform> transform;

		unsigned int i = 0;
		for (Entity & entity : m_entities.entities_with_components(render, transform))
		{
			if (render->shape->getGlobalBounds().contains(point))
			{
				//Update the GUI display
				info = { render->name, i, true, entity };
				info.changeName(render->name);
				return true;
			}
			i++;
		}
		return false;
	}

	std::vector<std::string> & Scene::getLayers()
	{
		return m_layers;
	}

	Entity Scene::getEntity(const std::string & name)
	{
		ComponentHandle<Render> render;
		Entity found;

		for (Entity & entity : m_entities.entities_with_components(render))
			if (render->name == name)
				found = entity;

		return found;
	}

	EntityManager & Scene::getEntities()
	{
		return m_entities;
	}
}