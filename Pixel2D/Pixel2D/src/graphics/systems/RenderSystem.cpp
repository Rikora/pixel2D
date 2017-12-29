#include "RenderSystem.hpp"
#include "..\components\Render.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

namespace px
{
	RenderSystem::RenderSystem(sf::RenderTarget & target) : target(target)
	{
	}

	void RenderSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
	{
		ComponentHandle<Render> render;

		for (Entity entity : es.entities_with_components(render))
		{
			target.draw(*render->shape.get());	
			target.draw(getBoundingRect(render->shape->getGlobalBounds()));
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