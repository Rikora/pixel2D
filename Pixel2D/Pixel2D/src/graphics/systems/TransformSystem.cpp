#include "TransformSystem.hpp"
#include "../components/Render.hpp"
#include "../components/Transform.hpp"

namespace px
{
	TransformSystem::TransformSystem()
	{
	}

	void TransformSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
	{
		ComponentHandle<Render> render;
		ComponentHandle<Transform> transform;

		for (Entity entity : es.entities_with_components(render, transform))
		{
			render->shape->setPosition(transform->position);
			render->shape->setScale(transform->scale);
			render->shape->setRotation(transform->rotation);
		}
	}
}