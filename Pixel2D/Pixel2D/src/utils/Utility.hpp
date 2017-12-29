#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

namespace px
{
	namespace utils
	{
		static unsigned int selected = 0;
		static unsigned int circleCounter = 0;

		std::string generateName(const std::string & name, unsigned int & counter);
		sf::Vector2f lerp(sf::Vector2f start, sf::Vector2f end, float alpha);
		sf::Keyboard::Key toKey(const std::string key);
	}
}
