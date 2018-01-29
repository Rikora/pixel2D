#pragma once
#include <memory>
#include "SFMLDebugDraw.hpp"
#include <SFML/System/Vector2.hpp>

namespace px
{
	class Physics
	{
	public:
		Physics(sf::RenderTarget & target, const sf::Vector2f & gravity);

	public:
		void Update(float timestep);
		void DrawDebugData();

	public:
		b2World* GetWorld() const;

	private:
		SFMLDebugDraw m_debugDraw;
		std::unique_ptr<b2World> m_world;
	};
}
