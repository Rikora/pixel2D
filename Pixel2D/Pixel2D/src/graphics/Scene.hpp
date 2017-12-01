#pragma once
#include <entityx\entityx.h>
#include <SFML\Graphics\RenderTarget.hpp>

using namespace entityx;

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

	public:
		EntityManager & GetEntities();
		
	private:
		EntityManager m_entities;
		EventManager m_events;
		SystemManager m_systems;
	};
}