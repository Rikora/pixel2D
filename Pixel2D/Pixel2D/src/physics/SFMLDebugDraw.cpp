#include "SFMLDebugDraw.hpp"
#include <SFML/Graphics.hpp>
#include "Box2DConverters.hpp"
#include "../utils/Utility.hpp"

namespace px
{
	SFMLDebugDraw::SFMLDebugDraw(sf::RenderTarget & target) : m_target(target)
	{
	}

	sf::Color SFMLDebugDraw::GLColorToSFML(const b2Color & color, sf::Uint8 alpha)
	{
		return sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255), alpha);
	}

	void SFMLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		sf::ConvexShape polygon(vertexCount);

		for (int i = 0; i < vertexCount; i++)
		{
			sf::Vector2f transformedVec = utils::boxToSfVec(vertices[i]);
			polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
		}																								   // they still show up though.. but less frequently

		polygon.setOutlineThickness(-1.f);
		polygon.setFillColor(sf::Color::Transparent);
		polygon.setOutlineColor(sf::Color::Green);

		m_target.draw(polygon);
	}

	void SFMLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		sf::ConvexShape polygon(vertexCount);

		for (int i = 0; i < vertexCount; i++)
		{
			sf::Vector2f transformedVec = utils::boxToSfVec(vertices[i]);
			polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
		}																								   // they still show up though.. but less frequently

		polygon.setOutlineThickness(-1.f);
		polygon.setFillColor(sf::Color(sf::Color::Green.r, sf::Color::Green.g, sf::Color::Green.b, 60));
		polygon.setOutlineColor(sf::Color::Green);

		m_target.draw(polygon);
	}

	void SFMLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		sf::CircleShape circle(utils::boxToSfFloat(radius));

		circle.setOrigin(utils::boxToSfFloat(radius), utils::boxToSfFloat(radius));
		circle.setPosition(utils::boxToSfVec(center));
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineThickness(-1.f);
		circle.setOutlineColor(sf::Color::Green);

		m_target.draw(circle);
	}

	void SFMLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		sf::CircleShape circle(utils::boxToSfFloat(radius));
		circle.setOrigin(utils::boxToSfFloat(radius), utils::boxToSfFloat(radius));
		circle.setPosition(utils::boxToSfVec(center));
		circle.setFillColor(sf::Color(sf::Color::Green.r, sf::Color::Green.g, sf::Color::Green.b, 60));
		circle.setOutlineThickness(-1.f);
		circle.setOutlineColor(sf::Color::Green);

		b2Vec2 endPoint = center + radius * axis;
		sf::Vertex line[2] =
		{
			sf::Vertex(utils::boxToSfVec(center), sf::Color::Green),
			sf::Vertex(utils::boxToSfVec(endPoint), sf::Color::Green),
		};

		m_target.draw(circle);
		m_target.draw(line, 2, sf::Lines);
	}

	void SFMLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(utils::boxToSfVec(p1), sf::Color::Green),
			sf::Vertex(utils::boxToSfVec(p2), sf::Color::Green)
		};

		m_target.draw(line, 2, sf::Lines);
	}

	void SFMLDebugDraw::DrawTransform(const b2Transform& xf)
	{
		float lineLength = 0.4f;

		b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
		sf::Vertex redLine[] =
		{
			sf::Vertex(utils::boxToSfVec(xf.p), sf::Color::Red),
			sf::Vertex(utils::boxToSfVec(xAxis), sf::Color::Red)
		};

		b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
		sf::Vertex greenLine[] =
		{
			sf::Vertex(utils::boxToSfVec(xf.p), sf::Color::Green),
			sf::Vertex(utils::boxToSfVec(yAxis), sf::Color::Green)
		};

		m_target.draw(redLine, 2, sf::Lines);
		m_target.draw(greenLine, 2, sf::Lines);
	}

	void SFMLDebugDraw::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
	{

	}
}
