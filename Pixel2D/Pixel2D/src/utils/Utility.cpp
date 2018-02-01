#include "Utility.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "../physics/Box2DConverters.hpp"

namespace px
{
	namespace utils
	{
		std::string generateName(const std::string & name, unsigned int & counter)
		{
			const std::string n = name + std::to_string(counter); counter++;
			return n;
		}

		sf::Vector2f lerp(const sf::Vector2f & start, const sf::Vector2f & end, const float & alpha)
		{
			return (start * (1 - alpha) + end * alpha);
		}

		sf::Vector2f getMouseWorldPos(const sf::RenderTexture & texture, const sf::RenderWindow & window)
		{
			sf::Vector2i pos = sf::Mouse::getPosition(window) - sf::Vector2i(16, 50); //(16, 50) is docking offset
			sf::Vector2f worldPos = sf::Vector2f(texture.mapPixelToCoords(pos).x, window.mapPixelToCoords(pos).y);
			float yOffset = texture.getView().getCenter().y - (texture.getView().getSize().y / 2.f);
			worldPos = sf::Vector2f(worldPos.x, (-worldPos.y + texture.getView().getSize().y) + yOffset);
			return worldPos;
		}

		sf::Keyboard::Key toKey(const std::string & key)
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

		void centerOrigin(sf::Shape & shape)
		{
			sf::FloatRect bounds = shape.getLocalBounds();
			shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
		}
	}
}