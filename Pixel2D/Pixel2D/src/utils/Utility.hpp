#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <vector>

namespace sf
{
	class RenderWindow;
	class RenderTexture;
}

namespace px
{
	namespace utils
	{
		static unsigned int selected = 0;
		static unsigned int circleCounter = 1;
		static unsigned int rectangleCounter = 1;

		std::string generateName(const std::string & name, unsigned int & counter);
		sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f & end, const float & alpha);
		sf::Vector2f getMouseWorldPos(const sf::RenderTexture & texture, const sf::RenderWindow & window);
		sf::Keyboard::Key toKey(const std::string & key);
	}
}
