#include "RigidbodyShape.hpp"
#include <Box2D/Box2D.h>
#include "../physics/Box2DConverters.hpp"
#include <SFML/Graphics/Transform.hpp>

namespace px
{
	RigidbodyShape::RigidbodyShape(Collider colliderType, b2World* world) : m_world(world), m_colliderType(colliderType), m_localPosition()
	{
		createBody();
	}

	void RigidbodyShape::destroyBody()
	{
		if (m_body != nullptr)
		{
			m_world->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void RigidbodyShape::createBody()
	{
		//Create empty body
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		bodyDef.position = utils::sfToBoxVec(sf::Vector2f(0.f, 0.f)); //Default at origin
		bodyDef.type = b2_dynamicBody;
		m_body = m_world->CreateBody(&bodyDef);

		//Apply collider depending on the type
		if (m_colliderType == Collider::Circle)
		{
			b2CircleShape shape;
			shape.m_radius = utils::sfToBoxFloat(1.f);
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;
			m_body->CreateFixture(&fixtureDef);
		}
		else if (m_colliderType == Collider::Box)
		{
			b2PolygonShape shape;
			shape.SetAsBox(utils::sfToBoxFloat(1.f), utils::sfToBoxFloat(1.f), utils::sfToBoxVec(sf::Vector2f(0.f, 0.f)), 0.f);
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;
			m_body->CreateFixture(&fixtureDef);
		}
	}

	//Polygon colliders
	void RigidbodyShape::setTransform(const sf::Vector2f & position, const float & angle, const sf::Vector2f & scale)
	{
		m_size = scale;

		//Only adjust the size if we really have to?
		if (m_colliderType == Collider::Box && scale != sf::Vector2f())
		{
			//Create new body from scratch
			b2BodyDef bodyDef;
			b2FixtureDef fixtureDef;		
			destroyBody();

			//Create empty body
			bodyDef.position = utils::sfToBoxVec(position);
			bodyDef.type = b2_dynamicBody;
			m_body = m_world->CreateBody(&bodyDef);

			//Set collider
			b2PolygonShape shape;
			shape.SetAsBox(utils::sfToBoxFloat(m_size.x), utils::sfToBoxFloat(m_size.y), utils::sfToBoxVec(sf::Vector2f(0.f, 0.f)), utils::sfToBoxAngle(angle));
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;
			m_body->CreateFixture(&fixtureDef);
		}
		else
			m_body->SetTransform(utils::sfToBoxVec(position), utils::sfToBoxAngle(angle));
	}

	//Only used for circle colliders
	void RigidbodyShape::setTransform(const sf::Vector2f & position, const float & radius, const float & angle)
	{
		m_radius = radius;
		m_body->GetFixtureList()->GetShape()->m_radius = utils::sfToBoxFloat(m_radius);
		m_body->SetTransform(utils::sfToBoxVec(position), utils::sfToBoxAngle(angle));
	}

	RigidbodyShape::Collider RigidbodyShape::getColliderType() const
	{
		return m_colliderType;
	}

	b2Body* RigidbodyShape::getBody() const
	{
		return m_body;
	}

	sf::Vector2f & RigidbodyShape::getLocalPositionRef()
	{
		return m_localPosition;
	}

	sf::Vector2f RigidbodyShape::getLocalPosition() const
	{
		return m_localPosition;
	}

	sf::Vector2f RigidbodyShape::getWorldPosition() const
	{
		return utils::boxToSfVec(m_body->GetPosition());
	}

	sf::Vector2f & RigidbodyShape::getSizeRef()
	{
		return m_size;
	}

	sf::Vector2f RigidbodyShape::getSize() const
	{
		return m_size;
	}

	float RigidbodyShape::getRotation() const
	{
		return utils::boxToSfAngle(m_body->GetAngle());
	}

	float & RigidbodyShape::getRadiusRef()
	{
		return m_radius;
	}

	float RigidbodyShape::getRadius() const
	{
		return m_radius;
	}
}