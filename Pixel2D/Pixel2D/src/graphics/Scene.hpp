#pragma once
#include <entityx\entityx.h>

//Components
#include "components\Render.hpp"
#include "components\Transform.hpp"

using namespace entityx;

namespace sf
{
	class RenderTarget;
}

namespace px
{
	class Scene : public EntityX
	{
	public:
		enum class Shapes
		{
			CIRCLE,
		};

	public:
		struct ObjectInfo
		{
			ObjectInfo(){}
			ObjectInfo(const std::string & name, const sf::Vector2f & pos, const sf::Vector2f & scale, 
					   const float & rot, const unsigned int & i, const bool & picked) :
					   pickedName(name), position(pos), scale(scale), rotation(rot), selected(i), picked(picked) {}

			void changeName(const std::string & name)
			{
				nameChanger.clear(); nameChanger.resize(50);

				for (std::size_t p = 0; p < name.size(); ++p)
					nameChanger[p] = name[p];
			}

			unsigned int selected;
			bool picked = false;
			float rotation;
			std::string pickedName;
			sf::Vector2f position;
			sf::Vector2f scale;
			std::vector<char> nameChanger;
		};

	public:
		Scene(sf::RenderTarget & target);
		~Scene();

	public:
		void createEntity(const Scene::Shapes & shape, const sf::Vector2f & position, const std::string & name, ObjectInfo & info);
		void destroyEntity(const std::string & name);
		void destroyEntities();
		void updateName(std::string & cName, const std::string & nName);
		void updateTransform(const ObjectInfo & info);
		void updateTransformSystem(const double & dt);
		void updateRenderSystem(const double & dt);
		bool checkIntersection(const sf::Vector2f & point);
		bool checkIntersection(const sf::Vector2f & point, ObjectInfo & info);
		Entity getEntity(const std::string & name);

	public:
		EntityManager & getEntities();
		
	private:
		EntityManager m_entities;
		EventManager m_events;
		SystemManager m_systems;
	};
}