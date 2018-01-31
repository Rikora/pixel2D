#include "TransformSystem.hpp"
#include <Box2D/Dynamics/b2Body.h>
#include "../components/Render.hpp"
#include "../components/Transform.hpp"
#include "../components/Rigidbody.hpp"
#include <SFML/Graphics/Transform.hpp>

namespace px
{
	TransformSystem::TransformSystem()
	{
	}

	void TransformSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
	{
		ComponentHandle<Render> render;
		ComponentHandle<Transform> transform;

		//This probably have to change when there is a play button for simulation*
		for (Entity & entity : es.entities_with_components(render, transform))
		{
			render->shape->setPosition(transform->position);
			render->shape->setScale(transform->scale);
			render->shape->setRotation(transform->rotation);

			//After or before?
			if (entity.has_component<Rigidbody>())
			{
				const sf::Vector2f pos = transform->position + (entity.component<Rigidbody>()->body->getLocalPosition());
				entity.component<Rigidbody>()->body->setTransform(pos, transform->rotation);
			}
		}
	}
}