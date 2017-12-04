#pragma once
#include <memory>
#include <SFML\Graphics\Shape.hpp>

namespace px
{
	struct Render
	{
		explicit Render(std::unique_ptr<sf::Shape> shape, std::string name) : shape(std::move(shape)), name(name) {}

		std::unique_ptr<sf::Shape> shape;
		std::string name;
	};
}