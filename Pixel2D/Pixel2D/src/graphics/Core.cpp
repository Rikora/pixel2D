#include <graphics/Core.hpp>
#include <utils/Themes.hpp>
#include <utils/Utility.hpp>
#include <utils/Macros.hpp>
#include <utils/Log.hpp>
#include <utils/Console.hpp>
#include <utils/imguiSTL.hpp>
#include <imgui-SFML.h>
#include <imguidock.h>
#include <SFML/Window/Event.hpp> 
#include <Windows.h>
#include <physics/Box2DConverters.hpp>
#include <graphics/components/Script.hpp>

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
		m_scene->destroyEntities();
	}

	void Core::initialize()
	{
		//Window
		m_window.setPosition({ 125, 75 });
		m_window.setVerticalSyncEnabled(true);

		//Load resources
		//loadTextures();

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
		m_physicsWorld = std::make_unique<Physics>(m_sceneTexture, sf::Vector2f(0.f, -20.f));
		m_scene = std::make_unique<Scene>(m_sceneTexture, m_physicsWorld->GetWorld());

		//Test level
		//const int level[] =
		//{
		//	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		//	0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		//	1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2,
		//	0, 1, 0, 0, 2, 0, 2, 2, 2, 0, 1, 1, 1, 0, 0, 0,
		//	0, 1, 1, 0, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		//	0, 0, 1, 0, 2, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		//	2, 0, 1, 0, 2, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		//	0, 0, 1, 0, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
		//};

		////Tilemaps
		//m_tileMap = std::make_unique<utils::TileMap>(m_textures, Textures::ID::Sprite);
		//m_tileMap->loadMap(sf::Vector2u(32, 32), level, 16, 8);
		
		//Init functionality and scripts
		loadLua();
		loadLuaScripts();
	}

	void Core::loadTextures()
	{
		m_textures.LoadResource(Textures::ID::Sprite, "src/res/textures/tilesets/set.png");
	}

	void Core::loadLua()
	{
		//Usertypes
		lua.new_usertype<GameObject, std::string>("GameObject", "getPosX", &GameObject::getX, "getPosY", &GameObject::getY, 
												  "setPosition", &GameObject::setPosition);

		//Keyboard table
		sol::table kb = lua.create_named_table("keyboard");
		kb.set_function("isKeyPressed", [](const std::string & key)->bool { return sf::Keyboard::isKeyPressed(utils::toKey(key)); });
	}

	void Core::loadLuaScripts()
	{
		//Can each entity load the script and use the result from it?
		lua.script_file("../Debug/scripts/main.lua");
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
		//m_sceneTexture.draw(*m_tileMap);
		m_scene->updateRenderSystem(m_timestep.getStep());
		m_physicsWorld->DrawDebugData();
		m_sceneTexture.display();
	}

	void Core::processEvents()
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			//Close window
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				m_window.close();

			//Mouse strafing
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
				m_currentMousePos = sf::Mouse::getPosition(m_window);	

			//Mouse picking
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_isSceneHovered)
			{
				sf::Vector2f worldPos = utils::getMouseWorldPos(m_sceneTexture, m_window);

				//Check if the mouse picked an object
				if (m_scene->checkIntersection(worldPos, m_objectInfo))
				{	
					updateLayerItem(m_layerItem);
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
				m_sceneView.move(sf::Vector2f(static_cast<float>(m_deltaMouse.x), static_cast<float>(m_deltaMouse.y)));
			}

			//Input for scripts
			ComponentHandle<Script> script;
			for (Entity & entity : m_scene->getEntities().entities_with_components(script))
			{
				for (auto & s : script->scripts)
				{
					//Check type of script
					if (s == "PlayerController")
					{
						sol::function onInput = lua[entity.component<Render>()->name]["onInput"];
						onInput(lua[entity.component<Render>()->name], dt);
					}
				}
			}
		}

		float alpha = m_timestep.getInterpolationAlphaAsFloat();

		//Update for scripts
		ComponentHandle<Script> script;
		for (Entity & entity : m_scene->getEntities().entities_with_components(script))
		{
			for (auto & s : script->scripts)
			{
				//Check type of script
				if (s == "PlayerController")
				{
					sol::function onUpdate = lua[entity.component<Render>()->name]["onUpdate"];
					onUpdate(lua[entity.component<Render>()->name], alpha);
				}
			}
		}

		//Update engine systems
		//m_physicsWorld->Update(1 / 60.f);
		m_scene->updateTransformSystem(m_timestep.getStep());
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
				if (ImGui::MenuItem("Quit", "Escape")) { m_window.close(); }
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Circle"))
					{
						m_scene->createEntity(Scene::Shapes::Circle, m_sceneView.getCenter(),
							utils::generateName("Circle", utils::circleCounter), m_objectInfo);
						updateLayerItem(m_layerItem);
					}

					if (ImGui::MenuItem("Rectangle"))
					{
						m_scene->createEntity(Scene::Shapes::Rectangle, m_sceneView.getCenter(),
							utils::generateName("Rect", utils::rectangleCounter), m_objectInfo);
						updateLayerItem(m_layerItem);
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
			ImGui::SetNextWindowPosCenter();
			if (ImGui::Begin("Layer Settings", &m_showLayerSettings, ImVec2(500, 600), 1.f), ImGuiWindowFlags_NoMove)
			{
				layerSettingsMenu();
			}
			ImGui::End();
		}

		//Open popup for creation of objects
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && m_isSceneHovered)
			ImGui::OpenPopup("Add_Objects");

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

		//Popup for creation of various objects
		ImGui::SetNextWindowSize(ImVec2(100, 100));
		if (ImGui::BeginPopup("Add_Objects"))
		{
			ImGui::Text("Add");
			ImGui::Separator();
			if (ImGui::BeginMenu("Mesh"))
			{
				if (ImGui::MenuItem("Circle##One"))
				{
					m_scene->createEntity(Scene::Shapes::Circle, utils::getMouseWorldPos(m_sceneTexture, m_window),
										  utils::generateName("Circle", utils::circleCounter), m_objectInfo);
					updateLayerItem(m_layerItem);
				}

				if (ImGui::MenuItem("Rectangle##One"))
				{
					m_scene->createEntity(Scene::Shapes::Rectangle, utils::getMouseWorldPos(m_sceneTexture, m_window),
						utils::generateName("Rect", utils::rectangleCounter), m_objectInfo);
					updateLayerItem(m_layerItem);
				}

				ImGui::EndMenu();
			}
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
			ImGui::EndDockspace();
		}
		ImGui::End();
	}

	void Core::addLayer(std::vector<char> & layerHolder)
	{
		//Make sure that the new layer doesn't already exist
		auto valid = [](const std::string & l)->bool
		{
			for (const auto & layer : m_scene->getLayers())
				if (layer == l)
					return false;

			return true;
		};

		if (valid(layerHolder.data()) && layerHolder.data() != "")
		{
			m_scene->getLayers().emplace_back(layerHolder.data());
			layerHolder.clear(); layerHolder.resize(50);
		}
	}

	void Core::updateLayerItem(int & item)
	{
		//Pick the correct layer for the object and display in GUI
		for (unsigned i = 0; i < m_scene->getLayers().size(); ++i)
		{
			if (m_scene->getLayers()[i] == m_objectInfo.entity.component<Render>()->layer)
			{
				item = i;
				return;
			}
		}
	}

	void Core::layerSettingsMenu()
	{
		unsigned int i = 1;
		for (const auto & layer : m_scene->getLayers())
		{
			ImGui::Text("Layer: %s", layer.c_str());
			if (layer != "Default")
			{
				ImGui::SameLine(560);
				ImGui::PushID(i);
				if (ImGui::SmallButton("Delete"))
				{
					//Remove all entities which corresponds to the layer
					m_scene->destroyEntities(layer);

					//Remove the layer from the vector
					if (i != m_scene->getLayers().size() - 1)
						m_scene->getLayers()[i] = std::move(m_scene->getLayers().back());
					m_scene->getLayers().pop_back();
				}
				ImGui::PopID();
				i++;
			}
			ImGui::Separator();
		}

		//Add new layer
		static std::vector<char> layerName(50);
		if(ImGui::InputText("", layerName.data(), layerName.size(), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			addLayer(layerName);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+")) //Give the user ability to click a button for adding layers aswell
		{
			addLayer(layerName);
		}
		ImGui::Separator();
	}

	void Core::sceneDock()
	{
		m_isSceneHovered = ImGui::IsItemHovered();
		ImVec2 size = ImGui::GetContentRegionAvail();
		unsigned int width = m_sceneTexture.getSize().x;
		unsigned int height = m_sceneTexture.getSize().y;

		if (width != size.x || height != size.y)
		{
			//Scene view
			m_sceneTexture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
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
				m_objectInfo = { render->name, utils::selected, true, entity };
				m_objectInfo.changeName(render->name);
				updateLayerItem(m_layerItem);				
			}
			utils::selected++;
		}
	}

	void Core::inspectorDock()
	{
		static int floatPrecision = 3;

		if (m_objectInfo.picked)
		{
			//Change layer
			if (ImGui::Combo("Layer", &m_layerItem, m_scene->getLayers()))
				m_objectInfo.entity.component<Render>()->layer = m_scene->getLayers()[m_layerItem];

			//Change name of entity upon completion
			if (ImGui::InputText("Name", m_objectInfo.nameChanger.data(), m_objectInfo.nameChanger.size(), ImGuiInputTextFlags_EnterReturnsTrue))
				m_scene->updateName(m_objectInfo.pickedName, m_objectInfo.nameChanger.data());
			ImGui::Spacing();

			//Update transform of selected entity
			ImGui::SetNextTreeNodeOpen(true, 2);
			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::Spacing();
				ImGui::InputFloat2("Position", &m_objectInfo.entity.component<Transform>()->position.x, floatPrecision);
				ImGui::Spacing();
				ImGui::InputFloat2("Scale", &m_objectInfo.entity.component<Transform>()->scale.x, floatPrecision);
				ImGui::Spacing();
				ImGui::InputFloat("Rotation", &m_objectInfo.entity.component<Transform>()->rotation, 1.f, 0.f, floatPrecision);
			}
			ImGui::Spacing();

			//Rigidbody
			if (m_objectInfo.entity.has_component<Rigidbody>())
			{
				//Rigidbody info
				static bool open = true;
				ImGui::SetNextTreeNodeOpen(true, 2);
				if (ImGui::CollapsingHeader("Rigidbody", &open, ImGuiTreeNodeFlags_Leaf)) //Leaf node prevents closing bug
				{
					ImGui::Text("Content goes here...");
					ImGui::Spacing();

					//Seems like we have to remove the body first and add it again to be able to change the status...
					static bool staticStatus = false;
					staticStatus = m_objectInfo.entity.component<Rigidbody>()->body->getStaticStatus();

					ImGui::Checkbox("isStatic", &staticStatus);
					m_objectInfo.entity.component<Rigidbody>()->body->setStaticStatus(staticStatus);
					ImGui::Spacing();

					//Display info about collider type
					if (m_objectInfo.entity.component<Rigidbody>()->body->getColliderType() == RigidbodyShape::Collider::Circle)
					{
						ImGui::SetNextTreeNodeOpen(true, 2);
						if (ImGui::CollapsingHeader("Circle Collider"))
						{
							static sf::Vector2f localPos = sf::Vector2f();
							static float radius = 0.f;
							localPos = m_objectInfo.entity.component<Rigidbody>()->body->getLocalPosition();
							radius = m_objectInfo.entity.component<Rigidbody>()->body->getRadius();

							ImGui::Spacing();
							ImGui::InputFloat2("Center", &localPos.x, floatPrecision);
							m_objectInfo.entity.component<Rigidbody>()->body->setLocalPosition(localPos);
							ImGui::Spacing();
							ImGui::InputFloat("Radius", &radius, 0.1f, 0.f, floatPrecision);
							m_objectInfo.entity.component<Rigidbody>()->body->setRadius(radius);
						}					
						ImGui::Spacing();
					}
					else if (m_objectInfo.entity.component<Rigidbody>()->body->getColliderType() == RigidbodyShape::Collider::Box)
					{
						ImGui::SetNextTreeNodeOpen(true, 2);
						if (ImGui::CollapsingHeader("Box Collider"))
						{
							static sf::Vector2f localPos = sf::Vector2f();
							localPos = m_objectInfo.entity.component<Rigidbody>()->body->getLocalPosition();

							ImGui::Spacing();
							ImGui::InputFloat2("Center##One", &localPos.x, floatPrecision);
							m_objectInfo.entity.component<Rigidbody>()->body->setLocalPosition(localPos);
							/*ImGui::Spacing();
							ImGui::InputFloat2("Size", &m_objectInfo.entity.component<Rigidbody>()->body->getSizeRef().x, floatPrecision);*/
						}
						ImGui::Spacing();
					}
				}
				else
				{
					//Remove rigidbody from the entity
					m_objectInfo.entity.component<Rigidbody>()->body->destroyBody();
					m_objectInfo.entity.remove<Rigidbody>();
					open = true;
				}
			}

			//Scripts
			if (m_objectInfo.entity.has_component<Script>())
			{
				ImGui::SetNextTreeNodeOpen(true, 2);
				if (ImGui::CollapsingHeader("Script"))
				{
					ImGui::Spacing();
					unsigned int i = 0;
					for (const auto & script : m_objectInfo.entity.component<Script>()->scripts)
					{
						ImGui::Text("Script: %s", script.c_str());
						ImGui::SameLine(560);
						ImGui::PushID(i);
						if (ImGui::SmallButton("Delete"))
						{
							//Remove all entities which corresponds to the layer
							//m_scene->destroyEntities(layer);

							//Remove the layer from the vector
							if (i != m_objectInfo.entity.component<Script>()->scripts.size() - 1)
								m_objectInfo.entity.component<Script>()->scripts[i] = std::move(m_objectInfo.entity.component<Script>()->scripts.back());
							m_objectInfo.entity.component<Script>()->scripts.pop_back();
						}
						ImGui::PopID();
						i++;
						ImGui::Separator();
					}

					//Add new script
					static std::vector<char> scriptName(50);
					if (ImGui::InputText("", scriptName.data(), scriptName.size(), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						//Check which kind of script was requested and give access to it
						if (std::string(scriptName.data()) == "PlayerController")
						{
							//Check better naming later on as we can't have the same object name for multiple scripts...
							std::string objName = m_objectInfo.entity.component<Render>()->name;
							std::string result = objName + " = " + scriptName.data() + ":new" + "('" + objName + "')";
							lua.script(result);
						}

						m_objectInfo.entity.component<Script>()->scripts.emplace_back(scriptName.data());
						scriptName.clear(); scriptName.resize(50);
					}
				}		
			}

			//Add components
			ImGui::Separator();
			ImGui::Spacing(); ImGui::Spacing();
			ImGui::InvisibleButton("Invis", ImVec2((ImGui::GetWindowContentRegionWidth() / 2.f) - 100.f, 0.f));
			ImGui::SameLine();
			if (ImGui::Button("Add Component", ImVec2(200.f, 0.f)))
				ImGui::OpenPopup("ComponentPopup");

			//Display the different components available
			ImGui::SetNextWindowSize(ImVec2(200, 100));
			if (ImGui::BeginPopup("ComponentPopup"))
			{
				displayComponents();
				ImGui::EndPopup();
			}	
		}
	}

	void Core::displayComponents()
	{
		ImGui::Text("Component");
		ImGui::Separator();

		if (ImGui::BeginMenu("Physics"))
		{
			if (ImGui::MenuItem("Circle Collider##One"))
			{
				if (!m_objectInfo.entity.has_component<Rigidbody>())
				{
					//Create circle collider
					auto rigidbody = std::make_unique<RigidbodyShape>(RigidbodyShape::Collider::Circle, m_physicsWorld->GetWorld());
					const float radius = m_objectInfo.entity.component<Render>()->shape->getGlobalBounds().width;
					rigidbody->setTransform(sf::Vector2f(0.f, 0.f), radius, 0.f);
					m_objectInfo.entity.assign<Rigidbody>(std::move(rigidbody));
				}
			}
			if (ImGui::MenuItem("Box Collider##One"))
			{
				if (!m_objectInfo.entity.has_component<Rigidbody>())
				{
					//Create box collider
					auto rigidbody = std::make_unique<RigidbodyShape>(RigidbodyShape::Collider::Box, m_physicsWorld->GetWorld());
					const float width = m_objectInfo.entity.component<Render>()->shape->getGlobalBounds().width;
					const float height = m_objectInfo.entity.component<Render>()->shape->getGlobalBounds().height;
					rigidbody->setTransform(sf::Vector2f(0.f, 0.f), 0.f, sf::Vector2f(width / 2.f, height / 2.f));
					m_objectInfo.entity.assign<Rigidbody>(std::move(rigidbody));
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Lua Script"))
		{
			if (!m_objectInfo.entity.has_component<Script>())
				m_objectInfo.entity.assign<Script>();
		}
	}
}

