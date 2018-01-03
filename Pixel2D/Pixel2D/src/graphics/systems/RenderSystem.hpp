#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics/Shape.hpp>

using namespace entityx;

namespace sf
{
	class RenderTarget;
	class RectangleShape;
}

namespace px
{
	class RenderSystem : public System<RenderSystem>
	{
	public:
		explicit RenderSystem(sf::RenderTarget & target, std::vector<std::string> & layers);

	public:
		void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	private:
		sf::RectangleShape getBoundingRect(const sf::FloatRect & rect);

	private:
		sf::RenderTarget & m_target;
		std::vector<std::string> & m_layers;
	};
}