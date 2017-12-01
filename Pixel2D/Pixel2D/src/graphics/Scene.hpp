#pragma once
#include <entityx\entityx.h>

using namespace entityx;

namespace px
{
	class Scene : public EntityX
	{
	public:
		Scene();
		~Scene();

	public:
		EntityManager & GetEntities();
		
	private:
		EntityManager m_entities;
		EventManager m_events;
		SystemManager m_systems;
	};
}