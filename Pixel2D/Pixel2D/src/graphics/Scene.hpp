#pragma once
#include <entityx\entityx.h>

//Components
#include "components\Render.hpp"

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
		Scene(sf::RenderTarget & target);
		~Scene();

	public:
		void destroyEntities();
		void updateSystems(double dt);
		Entity getEntity(const std::string name);

	public:
		EntityManager & getEntities();
		
	private:
		EntityManager m_entities;
		EventManager m_events;
		SystemManager m_systems;
	};
}