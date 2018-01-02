#pragma once
#include <memory>
#include <SFML\Graphics\Shape.hpp>

namespace px
{
	struct Render
	{
		explicit Render(std::unique_ptr<sf::Shape> shape, const std::string & name, const std::string & layer) : shape(std::move(shape)), name(name),
						layer(layer) {}

		std::unique_ptr<sf::Shape> shape;
		std::string name;
		std::string layer;
	};
}