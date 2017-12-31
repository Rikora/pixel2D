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
namespace px
{
	namespace utils
	{
		template <typename Resource, typename Identifier>
		class ResourceHolder;
	}
}

using TextureHolder = px::utils::ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = px::utils::ResourceHolder<sf::Font, Fonts::ID>;