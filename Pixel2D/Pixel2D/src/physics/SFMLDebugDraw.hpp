#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Config.hpp>

namespace sf
{
	class RenderTarget;
	class Color;
}

namespace px
{
	class SFMLDebugDraw : public b2Draw
	{
	public:
		SFMLDebugDraw(sf::RenderTarget & target);

	public:
		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
		virtual void DrawTransform(const b2Transform& xf) override;
		virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

	private:
		sf::Color GLColorToSFML(const b2Color &color, sf::Uint8 alpha = 255);

	private:
		sf::RenderTarget & m_target;
	};
}