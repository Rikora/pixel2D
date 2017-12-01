#pragma once
#include <SFML/System/Vector2.hpp>

namespace px
{
	namespace utils
	{
		sf::Vector2f linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha)
		{
			return (start * (1 - alpha) + end * alpha);
		}
	}
}
