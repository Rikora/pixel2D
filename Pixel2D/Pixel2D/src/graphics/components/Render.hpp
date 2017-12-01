#pragma once
#include <memory>
#include <SFML\Graphics\Shape.hpp>

namespace px
{
	struct Render
	{
		explicit Render(std::unique_ptr<sf::Shape> shape) : shape(std::move(shape)) {}

		std::unique_ptr<sf::Shape> shape;
	};
}