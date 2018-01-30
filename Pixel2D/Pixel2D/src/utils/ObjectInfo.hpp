#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <entityx/entityx.h>

using namespace entityx;

//Object for display info in GUI
namespace px
{
	struct ObjectInfo
	{
		ObjectInfo() {}
		ObjectInfo(const std::string & name, const sf::Vector2f & pos, const sf::Vector2f & scale,
			const float & rot, const unsigned int & i, const bool & picked, const std::string & layer, Entity entity) :
			pickedName(name), position(pos), scale(scale), rotation(rot), selected(i), picked(picked), layer(layer), entity(entity) {}

		void changeName(const std::string & name)
		{
			nameChanger.clear(); nameChanger.resize(50);

			for (unsigned p = 0; p < name.size(); ++p)
				nameChanger[p] = name[p];
		}

		std::string layer;
		unsigned int selected;
		bool picked = false;
		float rotation;
		std::string pickedName;
		sf::Vector2f position;
		sf::Vector2f scale;
		std::vector<char> nameChanger;
		Entity entity;
	};
}