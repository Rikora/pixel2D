#include <utils/TileMap.hpp>

namespace px
{
	namespace utils
	{
		TileMap::TileMap(const TextureHolder & textures, const Textures::ID & tileset) : m_textures(textures), m_tileset(tileset) {}

		bool TileMap::loadMap(sf::Vector2u tileSize, const int * tiles, unsigned int width, unsigned int height)
		{
			m_vertices.setPrimitiveType(sf::Quads);
			m_vertices.resize(width * height * 4);

			//Populate the vertex array, with one quad per tile
			for (unsigned int i = 0; i < width; ++i)
			{
				for (unsigned int j = 0; j < height; ++j)
				{
					//Get the current tile number
					int tileNumber = tiles[i + j * width];

					//Find its position in the tileset texture
					int tu = tileNumber % (m_textures.GetResource(m_tileset).getSize().x / tileSize.x);
					int tv = tileNumber / (m_textures.GetResource(m_tileset).getSize().x / tileSize.x);

					//Get a pointer to the current tile's quad
					sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

					//Define its 4 corners
					quad[0].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>(j * tileSize.y));
					quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>(j * tileSize.y));
					quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileSize.x), static_cast<float>((j + 1) * tileSize.y));
					quad[3].position = sf::Vector2f(static_cast<float>(i * tileSize.x), static_cast<float>((j + 1) * tileSize.y));

					//Define its 4 uv-coords
					quad[0].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>(tv * tileSize.y));
					quad[1].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>(tv * tileSize.y));
					quad[2].texCoords = sf::Vector2f(static_cast<float>((tu + 1) * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
					quad[3].texCoords = sf::Vector2f(static_cast<float>(tu * tileSize.x), static_cast<float>((tv + 1) * tileSize.y));
				}
			}

			return true;
		}

		void TileMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
		{
			//Apply resources and draw
			states.transform *= getTransform();
			states.texture = &m_textures.GetResource(m_tileset);
			target.draw(m_vertices, states);
		}
	}
}