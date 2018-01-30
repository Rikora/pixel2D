#include "TransformSystem.hpp"
#include "../components/Render.hpp"
#include "../components/Transform.hpp"
#include "../components/Rigidbody.hpp"

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
			if (entity.has_component<Rigidbody>())
				entity.component<Rigidbody>()->body->setTransform(transform->position, transform->rotation);

			render->shape->setPosition(transform->position);
			render->shape->setScale(transform->scale);
			render->shape->setRotation(transform->rotation);
		}
	}
}