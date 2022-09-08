#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/Inspector.h"
#include "Editor/Layer2D.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/NameComponent.h>
#include <Saz/Systems/LevelSystem.h>

#include <entt/entt.hpp>
#include "Saz/LevelComponent.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.inl"


class ExampleLayer : public Saz::Layer
{
public:
	ExampleLayer()
		: Layer("Placeholder")
	{
	}
};


Application::Application()
{
	//PushLayer(new ExampleLayer());
	PushLayer(new Layer2D());
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();

	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();
	m_EntityWorld.RegisterSystem<ecs::Inspector>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
}

void Application::Destroy()
{
	Saz::Application::Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();
}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
