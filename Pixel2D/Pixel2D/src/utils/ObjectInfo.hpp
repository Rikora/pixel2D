#pragma once
#include <vector>
#include <string>
#include <entityx/entityx.h>

using namespace entityx;

//Object for display info in GUI
namespace px
{
	struct ObjectInfo
	{
		ObjectInfo() {}
		ObjectInfo(const std::string & name, const unsigned int & i, const bool & picked, Entity entity) :
			pickedName(name), selected(i), picked(picked), entity(entity) {}

		void changeName(const std::string & name)
		{
			nameChanger.clear(); nameChanger.resize(50);

			for (unsigned p = 0; p < name.size(); ++p)
				nameChanger[p] = name[p];
		}

		unsigned int selected;
		bool picked = false;
		std::string pickedName;
		std::vector<char> nameChanger;
		Entity entity;
	};
}