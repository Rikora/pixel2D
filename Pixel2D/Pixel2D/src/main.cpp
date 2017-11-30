#include "utils\Macros.hpp"
#include <SFML\Graphics.hpp>
#include <Thor\Input.hpp>
#include <Kairos\Timestep.hpp>
#include <Kairos\FpsLite.hpp>

sf::Vector2f linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha)
{
	return (start * (1 - alpha) + end * alpha);
}

int main()
{
	const std::string windowTitle = "Timestep example";
	sf::RenderWindow window(sf::VideoMode(800, 600), windowTitle.c_str(), sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	sf::CircleShape circle(25.f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setFillColor(sf::Color(192, 128, 192));
	circle.setOutlineColor(sf::Color(64, 255, 192));
	circle.setOutlineThickness(-5.f);
	const float movementSpeed{ 250.f }; // pixels per second
	sf::Vector2f currentCirclePosition{ window.getSize() / 2u };
	sf::Vector2f previousCirclePosition = currentCirclePosition;

	kairos::FpsLite fps;

	kairos::Timestep timestep;
	timestep.setStep(1.0 / 60.0); // 'physics' timestep is one fifth of a second, or 5 frames per second.
	timestep.setMaxAccumulation(0.25); // set maximum time processed at once to a quarter of a second. if time passed is greater than this amount, the extra is discarded.
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		fps.update();

		timestep.addFrame(); // add frame to timestep each cycle
		while (timestep.isUpdateRequired()) // this is true as long as there are unprocessed timesteps.
		{
			previousCirclePosition = currentCirclePosition;
			float dt = timestep.getStepAsFloat();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // move up
				currentCirclePosition.y -= movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // move down
				currentCirclePosition.y += movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // move left
				currentCirclePosition.x -= movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // move right
				currentCirclePosition.x += movementSpeed * dt;
		}

		float interpolationAlpha = timestep.getInterpolationAlphaAsFloat(); // the interpolation alpha is how much the unprocessed time is of a step.
		circle.setPosition(linearInterpolation(previousCirclePosition, currentCirclePosition, interpolationAlpha));

		// shows information in the window title bar
		std::string infoTitle = windowTitle;
		infoTitle += " || Fps: " + std::to_string(fps.getFps());

		window.setTitle(infoTitle.c_str());

		window.clear();
		window.draw(circle);
		window.display();
	}

	return EXIT_SUCCESS;
}


//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(600, 480), "SFML works!");
//	sf::CircleShape shape(100.f);
//	shape.setFillColor(sf::Color::Green);
//
//	//Thor
//	thor::ActionMap<std::string> map;
//	thor::ActionMap<std::string>::CallbackSystem system;
//
//	map["Quit"] = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
//	map["Shoot"] = thor::Action(sf::Keyboard::E, thor::Action::PressOnce);
//
//	//Callback
//	system.connect("Shoot", [](thor::ActionContext<std::string> context) { PRINT("Hello there")} );
//
//	while (window.isOpen())
//	{
//		//Clear events from last frame
//		map.clearEvents();
//
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			map.pushEvent(event);
//
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		map.invokeCallbacks(system, nullptr);
//
//		if (map.isActive("Quit"))
//			window.close();
//
//		window.clear();
//		window.draw(shape);
//		window.display();
//	}
//
//	return 0;
//}