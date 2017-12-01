#pragma once
#include <entityx\entityx.h>
#include <SFML\Graphics\RenderTarget.hpp>

using namespace entityx;

namespace px
{
	class RenderSystem : public System<RenderSystem>
	{
	public:
		explicit RenderSystem(sf::RenderTarget & target);

	public:
		void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	private:
		sf::RenderTarget & target;	
	};
}