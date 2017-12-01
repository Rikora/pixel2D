#include "Scene.hpp"

namespace px
{
	Scene::Scene() : m_entities(m_events), m_systems(m_entities, m_events)
	{
	}

	Scene::~Scene()
	{
	}

	EntityManager & Scene::GetEntities()
	{
		return m_entities;
	}
}