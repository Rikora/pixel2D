#include "Core.hpp"
#include "..\utils\Themes.hpp"
#include "..\utils\Utility.hpp"
#include "..\utils\Macros.hpp"
#include "..\utils\Log.hpp"
#include "..\utils\Console.hpp"
#include <imgui-SFML.h>
#include <imguidock.h>
#include <SFML\Window\Event.hpp> 

namespace px
{
	std::unique_ptr<Scene> Core::m_scene;
	int Core::m_layerItem = 0;
	bool Core::m_showLayerSettings = false;

	Core::Core() : m_window(sf::VideoMode(1400, 900), "Pixel2D", sf::Style::Close), m_isSceneHovered(false)
	{
		initialize();
	}

	Core::~Core()
	{
		ImGui::SFML::Shutdown();
	}

	void Core::initialize()
	{
		//Window
		m_window.setPosition({ 125, 75 });
		m_window.setVerticalSyncEnabled(true);

		//Load resources
		loadTextures();

		//Render texture
		m_sceneTexture.create(m_window.getSize().x, m_window.getSize().y);

		//Timestep
		m_timestep.setStep(1.0 / 60.0);
		m_timestep.setMaxAccumulation(0.25); //Maximum time processed

		//GUI
		DarkWhiteTheme(true, 0.9f);
		ImGui::SFML::Init(m_window);
		ImGui::GetIO().MouseDrawCursor = true;

		//Scene
		m_scene = std::make_unique<Scene>(m_sceneTexture);

		loadLua();
		loadLuaScripts();

		//Doesn't work for multiple scripts if same name,
		//so need to have a table for each script?
		//Start for scripts
		//lua["onStart"]();
	}

	void Core::loadTextures()
	{
		m_textures.LoadResource(Textures::ID::Sprite, "src/res/textures/tilesets/tileset.png");
	}

	void Core::loadLua()
	{
		//Usertypes
		lua.new_usertype<Object>("Object", "get", &Object::getEntity, "getPosX", &Object::getX, "getPosY", &Object::getY, "setPosition", &Object::setPosition);

		//Keyboard table
		sol::table kb = lua.create_named_table("keyboard");
		kb.set_function("isKeyPressed", [](const std::string key)->bool { return sf::Keyboard::isKeyPressed(utils::toKey(key)); });
	}

	void Core::loadLuaScripts()
	{
		lua.script_file("src/res/scripts/circle.lua");
	}

	void Core::run()
	{
		sf::Clock guiClock;
		while (m_window.isOpen())
		{
			processEvents();
			update();
			ImGui::SFML::Update(m_window, guiClock.restart());
			updateGUI();

			std::string infoTitle;
			infoTitle += "Pixel2D || Fps: " + std::to_string(m_fps.getFps());
			m_window.setTitle(infoTitle.c_str());
			m_window.clear();
			ImGui::SFML::Render(m_window);
			m_window.display();
		}
	}

	void Core::render()
	{
		m_sceneTexture.clear(sf::Color::Black);
		m_sceneTexture.setView(m_sceneView);
		m_scene->updateRenderSystem(m_timestep.getStep());
		m_sceneTexture.display();
	}

	void Core::processEvents()
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				m_window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
				m_currentMousePos = sf::Mouse::getPosition(m_window);	

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_isSceneHovered)
			{
				sf::Vector2f worldPos = utils::getMouseWorldPos(m_sceneTexture, m_window);

				//Check if the mouse picked an object
				if (m_scene->checkIntersection(worldPos, m_objectInfo))
				{
					for (std::size_t i = 0; i < m_scene->getLayers().size(); ++i)
						if (m_scene->getLayers()[i] == m_objectInfo.layer)
							m_layerItem = i;
					gameLog.print("Intersected\n");
				}
				else
					m_objectInfo.picked = false;
			}
		}
	}

	void Core::update()
	{
		m_fps.update();
		m_timestep.addFrame(); //Add frame each cycle
		while (m_timestep.isUpdateRequired()) //If there are unprocessed timesteps
		{
			m_previousMousePos = m_currentMousePos;
			float dt = m_timestep.getStepAsFloat();

			//Add strafing for mouse
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && m_isSceneHovered)
			{
				m_currentMousePos = sf::Mouse::getPosition(m_window);
				m_deltaMouse = sf::Vector2i(m_currentMousePos.x - m_previousMousePos.x, m_previousMousePos.y - m_currentMousePos.y);
				m_sceneView.move(sf::Vector2f((float)m_deltaMouse.x, (float)m_deltaMouse.y));
			}

			//Input for scripts
			//lua["onInput"](dt);
		}

		float alpha = m_timestep.getInterpolationAlphaAsFloat();

		m_scene->updateTransformSystem(m_timestep.getStep());

		//Update for scripts
		//lua["onUpdate"](alpha);
	}

	void Core::updateGUI()
	{
		//Display different cursor on drag
		if(ImGui::IsMouseDown(sf::Mouse::Middle) && m_isSceneHovered)
			ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Move);

		//Menu
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::BeginMenu("Open Recent"))
				{
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if (ImGui::BeginMenu("More.."))
					{
						ImGui::MenuItem("Hello");
						ImGui::MenuItem("Sailor");
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Disabled", false)) // Disabled
				{
					IM_ASSERT(0);
				}
				if (ImGui::MenuItem("Quit", "Escape")) { m_window.close(); }
				ImGui::EndMenu();
			}

			/*if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Show Grid", NULL, &m_displayInfo.showGrid)) {}
				if (ImGui::MenuItem("Show FPS", NULL, &m_displayInfo.showFPS)) {}
				if (ImGui::MenuItem("Show Position", NULL, &m_displayInfo.showCameraPosition)) {}
				if (ImGui::MenuItem("Show Diagnostics", NULL, &m_displayInfo.showDiagnostics)) {}
				if (ImGui::MenuItem("Show Debug Shapes", NULL, &m_displayInfo.showDebugDraw)) {}
				ImGui::EndMenu();
			}*/

			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Circle"))
					{
						m_scene->createEntity(Scene::Shapes::CIRCLE, m_sceneView.getCenter(),
							utils::generateName("Circle", utils::circleCounter), m_objectInfo);
						
						for (std::size_t i = 0; i < m_scene->getLayers().size(); ++i)
							if (m_scene->getLayers()[i] == m_objectInfo.layer)
								m_layerItem = i;
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Layers", nullptr, &m_showLayerSettings);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//Layers settings menu
		if (m_showLayerSettings)
		{
			//ImGui::SetNextWindowPosCenter();
			//if (ImGui::Begin("Layer Settings", &m_showLayerSettings, ImVec2(500, 600), 1.f), ImGuiWindowFlags_NoMove)
			//{
			//	unsigned int i = 1;
			//	for (auto c : m_scene->getLayers())
			//	{
			//		ImGui::Text("Layer: %s", c);		
			//		if (c != "0")
			//		{
			//			ImGui::SameLine(400);
			//			ImGui::PushID(i);
			//			if (ImGui::SmallButton("Delete"))
			//			{
			//				//TODO: pop correct index
			//			}
			//			ImGui::PopID();
			//			++i;
			//		}
			//		ImGui::Separator();
			//	}
			//	std::string l = std::to_string(m_scene->getLayers().size());
			//	if (ImGui::SmallButton("+"))
			//		m_scene->getLayers().push_back(l.c_str());
			//}
			//ImGui::End();
		}

		//Open popup for delete
		if (m_objectInfo.picked && sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
			ImGui::OpenPopup("Delete?");

		//Remove the picked entity if the user agrees
		if (ImGui::BeginPopupModal("Delete?", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Are you sure you want to delete '%s'?\nThis action can't be undone.\n\n", m_objectInfo.pickedName.c_str());
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				m_scene->destroyEntity(m_objectInfo.pickedName);
				m_objectInfo.picked = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();

			if (ImGui::Button("No", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		//Show world position of mouse 
		ImGui::SetNextWindowPos(ImVec2(static_cast<float>(m_window.getSize().x - 200u), static_cast<float>(m_window.getSize().y - 480u)));
		if (!ImGui::Begin("Mouse overlay", nullptr, ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		if(m_isSceneHovered)
			m_worldPos = utils::getMouseWorldPos(m_sceneTexture, m_window);
		ImGui::Text("(%.3f, %.3f)     ", m_worldPos.x, m_worldPos.y);
		ImGui::End();

		//Docking system
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		const ImGuiWindowFlags flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoTitleBar);
		const float oldWindowRounding = ImGui::GetStyle().WindowRounding; ImGui::GetStyle().WindowRounding = 0;
		const bool visible = ImGui::Begin("Docking system", nullptr, ImVec2(0, 0), 1.0f, flags);
		ImGui::GetStyle().WindowRounding = oldWindowRounding;
		ImGui::SetWindowPos(ImVec2(0, 10));

		if (visible)
		{
			ImGui::BeginDockspace();

			ImGui::SetNextDock(ImGuiDockSlot_Left);
			if (ImGui::BeginDock("Scene"))
			{
				sceneDock();
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
				inspectorDock();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Left);
			if (ImGui::BeginDock("Hierarchy"))
			{
				ImGui::BeginChild("Entities");
				hierarchyDock();
				ImGui::EndChild();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Bottom);
			if (ImGui::BeginDock("Log"))
			{
				gameLog.draw();
			}
			ImGui::EndDock();

			ImGui::SetNextDock(ImGuiDockSlot_Tab);
			if (ImGui::BeginDock("Console"))
			{
				gameConsole.draw();
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
	}

	void Core::sceneDock()
	{
		m_isSceneHovered = ImGui::IsItemHovered();
		ImVec2 size = ImGui::GetContentRegionAvail();
		unsigned int width = m_sceneTexture.getSize().x;
		unsigned int height = m_sceneTexture.getSize().y;

		if (width != size.x || height != size.y)
		{
			m_sceneTexture.create((unsigned int)size.x, (unsigned int)size.y);
			m_sceneView.setCenter({ size.x / 2.f, size.y / 2.f });
			m_sceneView.setSize({ size.x, size.y });
		}

		render();
		ImGui::Image(m_sceneTexture.getTexture());
	}

	void Core::hierarchyDock()
	{
		utils::selected = 0;
		ComponentHandle<Render> render;

		for (Entity & entity : m_scene->getEntities().entities_with_components(render))
		{
			char label[128];
			sprintf(label, render->name.c_str());
			if (ImGui::Selectable(label, m_objectInfo.selected == utils::selected))
			{
				//Update entity information for GUI
				m_objectInfo = { render->name, render->shape->getPosition(), render->shape->getScale(),
					render->shape->getRotation(), utils::selected, true, render->layer };
				m_objectInfo.changeName(render->name);

				for (std::size_t i = 0; i < m_scene->getLayers().size(); ++i)
					if (m_scene->getLayers()[i] == m_objectInfo.layer)
						m_layerItem = i;
			}
			utils::selected++;
		}
	}

	void Core::inspectorDock()
	{
		int floatPrecision = 3;

		if (m_objectInfo.picked)
		{
			//Change layer
			if (ImGui::Combo("Layer", &m_layerItem, m_scene->getLayers().data(), m_scene->getLayers().size()))
				m_scene->updateLayer(m_objectInfo.pickedName, m_scene->getLayers()[m_layerItem]);

			//Change name of entity upon completion
			if (ImGui::InputText("Name", m_objectInfo.nameChanger.data(), m_objectInfo.nameChanger.size(), ImGuiInputTextFlags_EnterReturnsTrue))
				m_scene->updateName(m_objectInfo.pickedName, m_objectInfo.nameChanger.data());

			ImGui::Spacing();

			//Update transform of selected entity
			ImGui::SetNextTreeNodeOpen(true, 2);
			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::Spacing();
				ImGui::InputFloat2("Position", &m_objectInfo.position.x, floatPrecision);
				ImGui::Spacing();
				ImGui::InputFloat2("Scale", &m_objectInfo.scale.x, floatPrecision);
				ImGui::Spacing();
				ImGui::InputFloat("Rotation", &m_objectInfo.rotation, 1.f, 0.f, floatPrecision);
			}
			ImGui::Spacing();

			m_scene->updateTransform(m_objectInfo);
		}
	}
}