#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

namespace px
{
	namespace utils
	{
		class TileMap : public sf::Drawable, sf::Transformable
		{
		public:
			TileMap(const TextureHolder & textures, const Textures::ID & tileset);
			bool loadMap(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

		private:
			virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

		private:
			sf::VertexArray m_vertices;
			Textures::ID m_tileset;
			const TextureHolder & m_textures;
		};
	}
}