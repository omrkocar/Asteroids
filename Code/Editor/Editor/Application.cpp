#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/Inspector.h"
#include "Editor/EditorLayer.h"
#include "SceneEditor.h"

Application::Application()
	: Saz::Application("Saz Editor")
{
	//PushLayer(new EditorLayer());
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();
}

void Application::Destroy()
{
	Saz::Application::Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();

	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();
	m_EntityWorld.RegisterSystem<ecs::Inspector>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
	m_EntityWorld.RegisterSystem<ecs::SceneEditor>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
