#pragma once
#include <entityx\entityx.h>

using namespace entityx;

namespace sf
{
	class RenderTarget;
}

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