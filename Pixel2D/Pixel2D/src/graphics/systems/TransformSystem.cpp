#include <graphics/systems/TransformSystem.hpp>
#include <Box2D/Dynamics/b2Body.h>
#include <SFML/Graphics/Transform.hpp>

#include <graphics/components/Render.hpp>
#include <graphics/components/Transform.hpp>
#include <graphics/components/Rigidbody.hpp>

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

			if (entity.has_component<Rigidbody>())
			{
				if (entity.component<Rigidbody>()->body->getColliderType() == RigidbodyShape::Collider::Circle)
				{
					const sf::Vector2f pos = transform->position + (entity.component<Rigidbody>()->body->getLocalPosition());
					const float radius = entity.component<Rigidbody>()->body->getRadius();
					entity.component<Rigidbody>()->body->setTransform(pos, radius, transform->rotation);
				}
				else if (entity.component<Rigidbody>()->body->getColliderType() == RigidbodyShape::Collider::Box)
				{
					const sf::Vector2f pos = transform->position + (entity.component<Rigidbody>()->body->getLocalPosition());
					//const sf::Vector2f size = entity.component<Rigidbody>()->body->getSize();
					entity.component<Rigidbody>()->body->setTransform(pos, transform->rotation); //Updating the size needs a workaround
				}
			}
		}
	}
}