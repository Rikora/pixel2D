#include "utils\Macros.hpp"
#include <SFML\Graphics.hpp>
#include <Thor\Input.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 480), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	//Thor
	thor::ActionMap<std::string> map;
	thor::ActionMap<std::string>::CallbackSystem system;

	map["Quit"] = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
	map["Shoot"] = thor::Action(sf::Keyboard::E, thor::Action::PressOnce);

	//Callback
	system.connect("Shoot", [](thor::ActionContext<std::string> context) { PRINT("Hello there")} );

	while (window.isOpen())
	{
		//Clear events from last frame
		map.clearEvents();

		sf::Event event;
		while (window.pollEvent(event))
		{
			map.pushEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		map.invokeCallbacks(system, nullptr);

		if (map.isActive("Quit"))
			window.close();

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}