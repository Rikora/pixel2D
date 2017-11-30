#include "utils\Themes.hpp"
#include <imgui-SFML.h>
#include <imguidock.h>

#include "utils\Macros.hpp"
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
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
	sf::RenderWindow window(sf::VideoMode(1400, 900), windowTitle.c_str(), sf::Style::Close);
	window.setPosition({ 125, 75 });
	window.setVerticalSyncEnabled(true);

	//Imgui
	sf::Clock imguiClock;
	
	DarkWhiteTheme(true, 0.9f);
	ImGui::SFML::Init(window);

	//Render texture
	sf::RenderTexture texture;
	texture.create(window.getSize().x, window.getSize().y);
	texture.setSmooth(true);

	sf::CircleShape circle(25.f);
	circle.setOrigin(circle.getRadius(), circle.getRadius()); circle.setFillColor(sf::Color::Green);
	const float movementSpeed = 250.f; // pixels per second
	sf::Vector2f currentCirclePosition = sf::Vector2f(670.f, 233.f);
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
			ImGui::SFML::ProcessEvent(event);

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
				currentCirclePosition.y += movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // move down
				currentCirclePosition.y -= movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // move left
				currentCirclePosition.x -= movementSpeed * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // move right
				currentCirclePosition.x += movementSpeed * dt;
		}

		float interpolationAlpha = timestep.getInterpolationAlphaAsFloat(); // the interpolation alpha is how much the unprocessed time is of a step.
		circle.setPosition(linearInterpolation(previousCirclePosition, currentCirclePosition, interpolationAlpha));

		//Imgui
		ImGui::SFML::Update(window, imguiClock.restart());

		//Docking
		//Docking system
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		const ImGuiWindowFlags flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoTitleBar);
		const float oldWindowRounding = ImGui::GetStyle().WindowRounding; ImGui::GetStyle().WindowRounding = 0;
		const bool visible = ImGui::Begin("Docking system", NULL, ImVec2(0, 0), 1.0f, flags);
		ImGui::GetStyle().WindowRounding = oldWindowRounding;
		ImGui::SetWindowPos(ImVec2(0, 10));

		if (visible)
		{
			ImGui::BeginDockspace();

			ImGui::SetNextDock(ImGuiDockSlot_Left);
			if (ImGui::BeginDock("Scene"))
			{
				ImVec2 size = ImGui::GetContentRegionAvail();
				unsigned int width = texture.getSize().x;
				unsigned int height = texture.getSize().y;

				if (width != size.x || height != size.y)
					texture.create((unsigned int)size.x, (unsigned int)size.y);

				texture.clear(sf::Color::Black);
				texture.draw(circle);
				texture.display();

				//Draw the image/texture, filling the whole dock window
				ImGui::Image(texture.getTexture());
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Bottom);
			if (ImGui::BeginDock("Debug"))
			{
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Tab);
			if (ImGui::BeginDock("Inspector"))
			{
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Left);
			if (ImGui::BeginDock("Hierarchy"))
			{
				ImGui::BeginChild("Entities");
				ImGui::EndChild();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Bottom);
			if (ImGui::BeginDock("Log"))
			{
				//gameLog.Draw();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Tab);
			if (ImGui::BeginDock("Console"))
			{
				//gameConsole.Draw();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Tab);
			if (ImGui::BeginDock("Assets"))
			{
			}
			ImGui::EndDock();

			ImGui::EndDockspace();
		}
		ImGui::End();

		// shows information in the window title bar
		std::string infoTitle = windowTitle;
		infoTitle += " || Fps: " + std::to_string(fps.getFps());

		window.setTitle(infoTitle.c_str());
		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
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
