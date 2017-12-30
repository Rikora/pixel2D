#pragma once
#include <SFML\Graphics.hpp>

namespace Textures
{
	enum class ID
	{
		Sprite,
	};
}

namespace Fonts
{
	enum class ID
	{
		Main
	};
}

//Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;