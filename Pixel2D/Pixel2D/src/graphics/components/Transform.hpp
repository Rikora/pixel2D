#pragma once
#include <SFML\System\Vector2.hpp>

namespace px
{
	struct Transform
	{
		Transform(const sf::Vector2f & position, const sf::Vector2f & scale, const float & rotation) :
			position(position), scale(scale), rotation(rotation) {}

		sf::Vector2f position;
		sf::Vector2f scale;
		float rotation;
	};
}