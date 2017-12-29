#include "Utility.hpp"

namespace px
{
	namespace utils
	{
		std::string generateName(const std::string & name, unsigned int & counter)
		{
			const std::string n = name + std::to_string(counter); counter++;
			return n;
		}

		sf::Vector2f lerp(sf::Vector2f start, sf::Vector2f end, float alpha)
		{
			return (start * (1 - alpha) + end * alpha);
		}

		sf::Keyboard::Key toKey(const std::string key)
		{
			//Is there a cleaner way to do this with defines?
			if (key == "W" || key == "w")
				return sf::Keyboard::Key::W;
			if (key == "A" || key == "a")
				return sf::Keyboard::Key::A;
			if (key == "S" || key == "s")
				return sf::Keyboard::Key::S;
			if (key == "D" || key == "d")
				return sf::Keyboard::Key::D;
			if (key == "UP" || key == "up")
				return sf::Keyboard::Key::Up;
			if (key == "DOWN" || key == "down")
				return sf::Keyboard::Key::Down;
			if (key == "RIGHT" || key == "right")
				return sf::Keyboard::Key::Right;
			if (key == "LEFT" || key == "left")
				return sf::Keyboard::Key::Left;
			if (key == "SPACE" || key == "space")
				return sf::Keyboard::Key::Space;
			if (key == "ENTER" || key == "enter")
				return sf::Keyboard::Key::Return;

			return sf::Keyboard::Key::Unknown;
		}
	}
}