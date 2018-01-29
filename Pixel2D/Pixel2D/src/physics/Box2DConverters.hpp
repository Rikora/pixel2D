#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>

//Converters between Box2D and SFML
namespace px
{
	namespace utils
	{
		static float worldScale = 100.f;

		inline b2Vec2 sfToBoxVec(const sf::Vector2f& vec)
		{
			return b2Vec2(vec.x / worldScale, -vec.y / worldScale);
		}

		inline sf::Vector2f boxToSfVec(const b2Vec2& vec)
		{
			return sf::Vector2f(vec.x, -vec.y) * worldScale;
		}

		inline float sfToBoxFloat(float val)
		{
			return val / worldScale;
		}

		inline float boxToSfFloat(float val)
		{
			return val * worldScale;
		}

		inline float sfToBoxAngle(float degrees)
		{
			return -degrees * 0.0174533f;
		}

		inline float boxToSfAngle(float rads)
		{
			return -rads * 57.29578f;
		}
	}
}