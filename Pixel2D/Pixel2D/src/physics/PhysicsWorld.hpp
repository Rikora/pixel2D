//#pragma once
//#include <memory>
//#include "SFMLDebugDraw.hpp"
//#include "Box2DConverters.hpp"
//
//namespace px
//{
//	class PhysicsWorld
//	{
//	public:
//		static void Init()
//		{
//			world = std::make_unique<b2World>(utils::sfToBoxVec(sf::Vector2f(0.f, 1000.f))); //0, -20
//			world->SetDebugDraw(m_debugDraw.get());
//
//			m_debugDraw->SetFlags(b2Draw::e_shapeBit);
//		}
//
//		static void Update()
//		{
//			world->Step(1 / 60.f, 8, 3); //Step at 60 frames/second
//		}
//
//		static void DrawDebugData()
//		{
//			world->DrawDebugData();
//		}
//
//	public:
//		static std::unique_ptr<b2World> world;
//
//	private:
//		static std::unique_ptr<SFMLDebugDraw> m_debugDraw;
//	};
//}
