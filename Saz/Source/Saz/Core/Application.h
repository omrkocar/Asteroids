#pragma once

#include <Saz/Core/EntityWorld.h>
#include <Saz/Core/LayerStack.h>
#include <Saz/Core/WindowBase.h>
#include "Saz/ImGui/ImGuiLayer.h"
#include "Core/String.h"


namespace Saz
{
	class GameTime;
}

namespace Saz
{
	class Application
	{

	public:
		Application(const String& name = "Saz Engine");
		virtual ~Application();

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline WindowBase& GetWindow() { return *m_Window; }
		ecs::EntityWorld* GetWorld() { return &m_World; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	public:
		ecs::EntityWorld m_World;

	protected:
		virtual void Init();
		virtual void PostInit();
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);

		float m_LastFrameTime = 0.0f;

	protected:
		WindowBase* m_Window = nullptr;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = false;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

