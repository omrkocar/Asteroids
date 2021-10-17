#include "SazPCH.h"
#include "Application.h"

#include "Saz/CameraComponent.h"
#include "Saz/EntityWorld.h"
#include "Saz/GameTime.h"
#include "Saz/InputComponent.h"
#include "Saz/InputSystem.h"
#include "Saz/RenderSystem.h"
#include "Saz/ResourceManager.h"
#include "Saz/GLFW/Window.h"
#include "Saz/SFML/Window.h"
#include "Saz/LevelComponent.h"
#include "Saz/LevelSystem.h"
#include "Saz/MovementComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/NameComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"
#include "Saz/TransformSystem.h"
#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/Device.h"
#include "Saz/Vulkan/SwapChain.h"
#include "Saz/Vulkan/Model.h"


#include <GLFW/glfw3.h>

namespace
{
	void Triangles(
		std::vector<vulkan::Vertex>& vertices,
		int depth,
		vec2 left,
		vec2 right,
		vec2 top) 
	{
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			Triangles(vertices, depth - 1, left, leftRight, leftTop);
			Triangles(vertices, depth - 1, leftRight, right, rightTop);
			Triangles(vertices, depth - 1, leftTop, rightTop, top);
		}
	}
}

namespace Saz
{

	Application* Application::s_Instance = nullptr;

	const ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	Application::Application()
	{
		spd::Log::Init();

		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		
		m_ImGuiLog = new imgui::Log();

		// #todo Create all textures with a single call here.
		m_pResourceManager = new Saz::ResourceManager();
		
		/*{
			Saz::WindowProps windowProps ;
			windowProps.m_Title = "SFML Window";
			windowProps.m_Size = { static_cast<int>(Screen::width), static_cast<int>(Screen::height) };
			m_SFMLWindow = new sfml::Window(windowProps);
		}*/

		{
			glfwInit();

			Saz::WindowProps windowProps;
			windowProps.m_Title = "GLFW Window";
			windowProps.m_Size = { static_cast<int>(Screen::width), static_cast<int>(Screen::height) };
			m_GLFWWindow = new glfw::Window(windowProps);

			m_Device = std::make_unique<vulkan::Device>(*m_GLFWWindow);
		}
	}

	Application::~Application()
	{
		//delete m_SFMLWindow;
		delete m_GLFWWindow;
		delete m_ImGuiLog;
		delete m_pResourceManager;
		glfwTerminate();
	}

	void Application::Init()
	{
		//m_SFMLWindow->Init();
		m_EntityWorld.Init();
	}

	void Application::PostInit()
	{
		m_EntityWorld.PostInit();
	}

	void Application::Register()
	{
		m_EntityWorld.RegisterComponent<component::InputComponent>();
		m_EntityWorld.RegisterComponent<component::LevelComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::RenderComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();
		m_EntityWorld.RegisterComponent<component::CameraComponent>();

		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_SFMLWindow);
		m_EntityWorld.RegisterSystem<ecs::LevelSystem>(*m_pResourceManager);
		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_Device, *m_GLFWWindow);
		m_EntityWorld.RegisterSystem<ecs::TransformSystem>();
	}

	void Application::Destroy()
	{
		//m_SFMLWindow->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		m_EntityWorld.Update(gameTime);
		//m_ImGuiLog->Update();
	}

	void Application::Run(int argc, char* argv[])
	{
		Register();
		Init();
		PostInit();

		sf::Clock clock;
		Saz::GameTime gameTime;

		while (true)
		{
			gameTime.m_Time = clock.restart();
			gameTime.m_DeltaTime = gameTime.m_Time.asSeconds();
			gameTime.m_TotalTime += gameTime.m_DeltaTime;
			gameTime.m_Frame++;

			/*m_SFMLWindow->Update(gameTime);
			if (m_SFMLWindow->ShouldClose())
				break;*/

			m_GLFWWindow->Update(gameTime);
			if (m_GLFWWindow->ShouldClose())
				break;

			Update(gameTime);
		}
	}
}