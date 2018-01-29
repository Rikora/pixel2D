#include "Physics.hpp"
#include "Box2DConverters.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace px
{
	Physics::Physics(sf::RenderTarget & target, const sf::Vector2f & gravity) : m_debugDraw(target)
	{
		//Create the world and allow debug
		m_world = std::make_unique<b2World>(utils::sfToBoxVec(gravity)); //Gravity 
		m_world->SetDebugDraw(&m_debugDraw);
		m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	}

	void Physics::Update(float timestep)
	{
		m_world->Step(timestep, 8, 3); //Usually 60 frames/second
	}

	void Physics::DrawDebugData()
	{
		m_world->DrawDebugData();
	}

	b2World * Physics::GetWorld() const
	{
		return m_world.get();
	}
}