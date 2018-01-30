#include "RenderSystem.hpp"
#include "..\components\Render.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

namespace px
{
	RenderSystem::RenderSystem(sf::RenderTarget & target, std::vector<std::string> & layers) : m_target(target), m_layers(layers)
	{
	}

	void RenderSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
	{
		ComponentHandle<Render> render;

		for (const auto & layer : m_layers)
		{
			for (Entity & entity : es.entities_with_components(render))
			{
				if (render->layer == layer)
				{
					m_target.draw(*render->shape.get());
					//m_target.draw(getBoundingRect(render->shape->getGlobalBounds())); //Draw bounding rect of the shape
				}
			}
		}
	}

	sf::RectangleShape RenderSystem::getBoundingRect(const sf::FloatRect & rect)
	{
		sf::RectangleShape shape;

		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		return shape;
	}
}