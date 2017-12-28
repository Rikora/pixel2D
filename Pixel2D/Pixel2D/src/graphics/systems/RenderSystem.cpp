#include "RenderSystem.hpp"
#include "..\components\Render.hpp"
#include <SFML\Graphics\RenderTarget.hpp>

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
		}
	}
}